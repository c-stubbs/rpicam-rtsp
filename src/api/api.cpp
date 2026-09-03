#include "api.h"

Api::Api(const ApiConfig& config)
    : config_(config)
    , log_("api", config.log_level)
{
    server.Get("/test", 
               [this](const httplib::Request& req, httplib::Response& res)
               {
                    this->onGetTest(req, res);
               });

    server.listen("0.0.0.0", 1234);
}

void Api::onGetTest(const httplib::Request& req, httplib::Response& res)
{
    res.set_content("Hello World!", "text/plain");
}
