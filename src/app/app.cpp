#include "app.h"

#include <thread>
#include <csignal>

App::App(const AppConfig& config)
    : config_(config)
    , log_("app", config.log_level)
    , rpicam_(config.rpicam_vid_wrapper_config)
    , rtsp_server_(config.rtsp_server_config)
{
}

int App::run(int& argc, char *argv[])
{
    std::signal(SIGINT, signalHandler);

    if (!rpicam_.start())
    {
        log_.critical("rpicam failed to start");
        return 1;
    }

    gst_init(&argc, &argv);

    if (!rtsp_server_.start())
    {
        log_.critical("rtsp server failed to start");
        return 1;
    }

    while (running)
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        log_.debug("rpicam status: {}", rpicam_.isRunning() ? "running" : "stopped");
        log_.debug("rtsp server status: {}", rtsp_server_.isRunning() ? "running" : "stopped");
    }

    log_.info("stopping execution");

    rtsp_server_.stop();
    rpicam_.stop();

    log_.info("stopped cleanly");
    return 0;
}

void App::signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}
