#pragma once

#include <thread>

#include "httplib.h"
#include "api_config.h"
#include "logger.h"

class Api {

    public:
        Api(const ApiConfig& config);
        ~Api();

        bool start();
        void stop();

    private:
        std::jthread server_thread_;

        httplib::Server server;

        const ApiConfig config_;
        Logger log_;

        void onGetTest(const httplib::Request& req, httplib::Response& res);

};
