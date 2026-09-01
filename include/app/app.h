#pragma once

#include <atomic>

#include "rpicam_vid_wrapper.h"
#include "rtsp_server.h"
#include "logger.h"
#include "app_config.h"

class App {

    public:
        App(const AppConfig& config); 
        int run(int& argc, char *argv[]);
    
    private:
        static void signalHandler(int signal);
        inline static std::atomic<bool> running{true};
        
        RpiCamVidWrapper rpicam_;
        RTSPServer rtsp_server_;
        Logger log_;
        const AppConfig config_;
};
