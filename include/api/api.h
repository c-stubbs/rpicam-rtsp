#pragma once

#include <thread>

#include "api_config.h"
#include "logger.h"
#include "httplib.h"


class App; 

class Api {

    public:
        Api(const ApiConfig& config, App& app);
        ~Api();

        bool start();
        void stop();

    private:
        std::jthread server_thread_;

        httplib::Server server;

        const ApiConfig config_;
        Logger log_;
        App& app_;

        void onGetMarco(const httplib::Request& req, httplib::Response& res);
        void onGetCameraConfig(const httplib::Request& req, httplib::Response& res);

};
