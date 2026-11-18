#include "app.h"

#include <thread>
#include <csignal>

#include <toml++/toml.hpp>

App::App(const AppConfig& config)
    : config_(config)
    , log_("app", config.log_level)
    , thread_pool_(1, log_)
    , rpicam_(config.rpicam_vid_wrapper_config)
    , rtsp_server_(config.rtsp_server_config)
    , api_(config.api_config, *this, thread_pool_)
{
}

int App::run(int& argc, char *argv[])
{
    std::signal(SIGINT, signalHandler);

    if (config_.rpicam_vid_wrapper_config.run && !rpicam_.start())
    {
        log_.critical("rpicam failed to start");
        return 1;
    }

    gst_init(&argc, &argv);

    if (config_.rtsp_server_config.run && !rtsp_server_.start())
    {
        log_.critical("rtsp server failed to start");
        return 1;
    }

    if (config_.api_config.run && !api_.start())
    {
        log_.critical("api failed to start");
        return 1;
    }

    while (running)
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        log_.debug("rpicam status: {}", rpicam_.isRunning() ? "running" : "stopped");
        log_.debug("rtsp server status: {}", rtsp_server_.isRunning() ? "running" : "stopped");
    }

    log_.info("stopping execution");

    api_.stop();
    rtsp_server_.stop();
    rpicam_.stop();

    log_.info("stopped cleanly");
    return 0;
}

AppConfig App::getConfig() const
{
    return config_;
}

void App::setConfig(AppConfig config)
{
    config_ = config;
}

void App::writeConfig()
{
    // TODO: want this to read in the current file to a toml,
    // compare it to the active toml, prune all differences,
    // and only write the remaining. Would also like it to
    // retain comments
}

void App::stop()
{
    running = false;
}

void App::startRpiCamVid()
{ 
    rpicam_.start(config_.rpicam_vid_wrapper_config);
}

void App::stopRpiCamVid()
{
    rpicam_.stop();
}

void App::startRtspServer()
{
    rtsp_server_.start(config_.rtsp_server_config);
}

void App::stopRtspServer()
{
    rtsp_server_.stop();
}

void App::signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}
