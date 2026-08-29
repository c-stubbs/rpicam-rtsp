#include "rpicam_vid_wrapper.h"
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <string>

RpiCamVidWrapper::RpiCamVidWrapper() : 
    log_("rpicam_vid_wrapper","info")
{
}

RpiCamVidWrapper::~RpiCamVidWrapper()
{
    stop();
}

std::optional<pid_t> RpiCamVidWrapper::start()
{
    // If pid_ is already valid, the stream is already open 
    if (isRunning())
    {
        return pid_;
    }

    pid_t pid = fork();
    
    // Fork failed
    if (pid < 0)
    {
        log_.error("fork: {}", std::strerror(errno));
        return std::nullopt;
    }

    // Child branch
    if (pid == 0)
    { 
        std::vector<std::string> args = {
            "rpicam-vid",
            "-t", "0",
            "-v", "0",
            "--width", "1920",
            "--height", "1080",
            "--framerate", "15",
            "--bitrate", "1000000",
            "--inline",
            "-o", "udp://127.0.0.1:5000"
        };

        std::vector<char*> argv;

        for (auto& arg : args) {
            argv.push_back(arg.data());
        }

        argv.push_back(nullptr);

        execvp(argv[0], argv.data());

        // Only reached if execvp process fails
        log_.error("execvp: {}", std::strerror(errno));
        _exit(1);
    }

    log_.info("started");
    log_.debug("child pid: {}", pid);

    // Parent branch
    pid_ = std::optional<pid_t>(pid);
    return pid_;
}

void RpiCamVidWrapper::stop()
{
    if (!pid_)
    {
        return;
    }

    pid_t pid = *pid_;

    // Check whether it has already exited
    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);

    if (result == pid)
    {
        // It has already exited
        pid_.reset();
        return;
    }

    if (result == -1)
    {
        if (errno == ECHILD)
        {
            // Already been reaped
            pid_.reset();
            return;
        }
        log_.error("waitpd: {}", std::strerror(errno));
        pid_.reset();
        return;
    }

    // Child is still running. Ask to terminate.
    if (kill(pid, SIGTERM) == -1)
    {
        if (errno != ESRCH)
        {
            log_.error("kill: {}", std::strerror(errno));
        }

        pid_.reset();
        return;
    }

    // Wait for it to exit
    if (waitpid(pid, &status, 0) == -1)
    {
        if (errno != ECHILD)
        {
            log_.error("waitpid: {}", std::strerror(errno));
        }
    }

    pid_.reset();
}

bool RpiCamVidWrapper::isRunning()
{
    if (!pid_)
    {
        return false;
    }

    int status;

    pid_t result = waitpid(*pid_, &status, WNOHANG);

    if (result == 0)
    {
        // Child is still running
        return true;
    }

    if (result == *pid_)
    {
        // Child exited
        pid_.reset();
        return false;
    }

    if (errno == EINTR)
    {
        // Interupted by sig
        return true;
    }

    log_.error("waitpid: {}", std::strerror(errno));
    pid_.reset();
    return false;
}
