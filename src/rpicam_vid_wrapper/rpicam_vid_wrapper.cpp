#include "rpicam_vid_wrapper.h"
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <string>

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
        perror("fork");
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
        perror("execvp");
        _exit(1);
    }

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
        perror("waitpid");
        pid_.reset();
        return;
    }

    // Child is still running. Ask to terminate.
    if (kill(pid, SIGTERM) == -1)
    {
        if (errno != ESRCH)
        {
            perror("kill");
        }

        pid_.reset();
        return;
    }

    // Wait for it to exit
    if (waitpid(pid, &status, 0) == -1)
    {
        if (errno != ECHILD)
        {
            perror("waitpid");
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

    perror("waitpid");
    pid_.reset();
    return false;
}
