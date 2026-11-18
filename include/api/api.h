#pragma once

#include <thread>

#include "api_config.h"
#include "logger.h"
#include "httplib.h"

class App;
class ThreadPool;

class Api {

    public:
        Api(const ApiConfig& config, App& app, ThreadPool& thread_pool);
        ~Api();

        bool start();
        void stop();

    private:
        std::jthread server_thread_;
        
        httplib::Server server;

        const ApiConfig config_;
        Logger log_;
        App& app_;
        ThreadPool& thread_pool_;

        void onGetMarco(const httplib::Request& req, httplib::Response& res);
        
        void onGetCameraConfig(const httplib::Request& req, httplib::Response& res);
        void onPatchCameraConfig(const httplib::Request& req, httplib::Response& res);
        void onPostCameraStart(const httplib::Request& req, httplib::Response& res);
        void onPostCameraStop(const httplib::Request& req, httplib::Response& res);
        void onPostCameraRestart(const httplib::Request& req, httplib::Response& res);

        void onGetRtspConfig(const httplib::Request& req, httplib::Response& res);
        void onPatchRtspConfig(const httplib::Request& req, httplib::Response& res);
        void onPostRtspStart(const httplib::Request& req, httplib::Response& res);
        void onPostRtspStop(const httplib::Request& req, httplib::Response& res);
        void onPostRtspRestart(const httplib::Request& req, httplib::Response& res);
};
