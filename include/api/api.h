#pragma once

#include "httplib.h"
#include "api_config.h"
#include "logger.h"

class Api {

    public:
        Api(const ApiConfig& config);

        bool start();

    private:
        httplib::Server server;

        const ApiConfig config_;
        Logger log_;

        void onGetTest(const httplib::Request& req, httplib::Response& res);

};
