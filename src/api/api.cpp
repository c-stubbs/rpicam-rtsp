#include "api.h"

#include "app.h"
#include "models.h"

Api::Api(const ApiConfig& config, App& app)
    : config_(config)
    , log_("api", config.log_level)
    , app_(app)
{
}

Api::~Api()
{
    stop();
}

bool Api::start()
{
    server.Get("/api/v1/marco", 
               [this](const httplib::Request& req, httplib::Response& res)
               {
                    this->onGetMarco(req, res);
               });

    server.Get("/api/v1/camera/config", 
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onGetCameraConfig(req, res);
                });

    server_thread_ = std::jthread([this](){
        server.listen("0.0.0.0", 8080);
    });

    return true;
}

void Api::stop()
{
    server.stop();
 
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }
}

void Api::onGetMarco(const httplib::Request& req, httplib::Response& res)
{
    res.set_content("Polo!", "text/plain");
}

void Api::onGetCameraConfig(const httplib::Request& req, httplib::Response& res)
{
  RpiCamVidWrapperConfig camera_config = app_.getCameraConfig();

  Models::CameraConfig config;
  config.height = camera_config.height;
  config.width = camera_config.width;
  config.fps = camera_config.framerate;

  res.set_content(config.dump(), "application/json");
}
