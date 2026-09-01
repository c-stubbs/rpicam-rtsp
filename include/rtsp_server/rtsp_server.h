#pragma once

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <glib.h>
#include <thread>

#include "logger.h"
#include "rtsp_server_config.h"

class RTSPServer {
    
    public:
        RTSPServer(const RtspServerConfig& config);
        ~RTSPServer();

        bool start();
        void stop();

        bool isRunning();
        
    private:
        GMainLoop* loop_ = nullptr;
        GstRTSPServer* server_ = nullptr;
        guint server_id_ = 0;
        std::string address_ = "0.0.0.0";
        int port_ = 8554;
        int udp_port_ = 5000;
        std::string mount_point_ = "/stream";
        std::thread loop_thread_;

        Logger log_;
        const RtspServerConfig config_;
};
