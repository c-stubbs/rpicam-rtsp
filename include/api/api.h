#pragma once

#include <thread>

#include "httplib.h"
#include "api_config.h"
#include "logger.h"

// Forward declare App to break circular dependency
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

        void onGetTest(const httplib::Request& req, httplib::Response& res);

};
