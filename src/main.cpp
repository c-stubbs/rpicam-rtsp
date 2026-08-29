#include <atomic>
#include <csignal>
#include "rpicam_vid_wrapper.h"
#include "rtsp_server.h"
#include "logger.h"

std::atomic<bool> running{true};

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}

int main(int argc, char *argv[])
{
    std::signal(SIGINT, signalHandler);

    Logger log("main", "debug");

    RpiCamVidWrapper rpicam;
    if (!rpicam.start())
    {
        log.error("rpicam failed to start");
        return 1;
    }

    gst_init(&argc, &argv);

    RTSPServer rtsp_server;
    if (!rtsp_server.start())
    {
        log.error("rtsp server failed to start");
        return 1;
    }

    while(running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        log.debug("rpicam status: {}", rpicam.isRunning() ? "running" : "stopped");
        log.debug("rtsp server status: {}", rtsp_server.isRunning() ? "running" : "stopped");
    }

    log.info("stopping execution");
    
    rtsp_server.stop();
    rpicam.stop();

    log.info("finished cleanly");
}

