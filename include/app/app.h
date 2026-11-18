#pragma once

#include <atomic>

#include "rpicam_vid_wrapper.h"
#include "rtsp_server.h"
#include "api.h"
#include "logger.h"
#include "app_config.h"
#include "thread_pool.h"

class App {

    public:
        App(const AppConfig& config); 
        int run(int& argc, char *argv[]);

        AppConfig getConfig() const;
        void setConfig(AppConfig config);
        void writeConfig();
        void stop();
        
        void startRpiCamVid();
        void stopRpiCamVid();

        void startRtspServer();
        void stopRtspServer();
    
        // App API
        /**
         * void saveConfig(); // write the current config to file
         * void stop(); // stop the app, let systemd restart it
         */

    private:
        static void signalHandler(int signal);
        inline static std::atomic<bool> running{true};
        
        ThreadPool thread_pool_;
        RpiCamVidWrapper rpicam_;
        RTSPServer rtsp_server_;
        Api api_;
        Logger log_;
        AppConfig config_;

};
