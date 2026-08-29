#include <atomic>
#include <csignal>
#include "rpicam_vid_wrapper.h"
#include "rtsp_server.h"

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
    RpiCamVidWrapper rpicam;
    if (!rpicam.start())
    {
        return 1;
    }

    // KEEP THIS HERE
    gst_init(&argc, &argv);

    RTSPServer rtsp_server;
    if (!rtsp_server.start())
    {
        return 1;
    }

    while(running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    rtsp_server.stop();
    rpicam.stop();
}

