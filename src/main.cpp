#include <iostream>
#include "rpicam_vid_wrapper.h"
#include "rtsp_server.h"

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

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

