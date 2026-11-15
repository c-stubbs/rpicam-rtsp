#include "api.h"

#include "app.h"
#include "httplib.h"
#include "models.h"
#include "thread_pool.h"

Api::Api(const ApiConfig& config, App& app, ThreadPool& thread_pool)
    : config_(config)
    , log_("api", config.log_level)
    , app_(app)
    , thread_pool_(thread_pool)
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

    server.Patch("/api/v1/camera/config", 
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onPatchCameraConfig(req, res);
                });
    
    server.Post("/api/v1/camera/start", 
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onPostCameraStart(req, res);
                });
   
    server.Post("/api/v1/camera/stop", 
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onPostCameraStop(req, res);
                });

    server.Post("/api/v1/camera/restart", 
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onPostCameraRestart(req, res);
                });

    server_thread_ = std::jthread([this](){
        server.listen("0.0.0.0", 8080);
    });

    log_.info("started");

    return true;
}

void Api::stop()
{
    server.stop();
 
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }

    log_.info("stopped");
}

void Api::onGetMarco(const httplib::Request& req, httplib::Response& res)
{
    res.set_content("Polo!", "text/plain");
}

void Api::onGetCameraConfig(const httplib::Request& req, httplib::Response& res)
{
  auto camera_config = app_.getConfig().rpicam_vid_wrapper_config;

  Models::CameraConfig config;
  config.height = camera_config.height;
  config.width = camera_config.width;
  config.fps = camera_config.framerate;

  res.set_content(config.dump(), "application/json");
}

void Api::onPatchCameraConfig(const httplib::Request& req, httplib::Response& res)
{
    auto app_config = app_.getConfig();
    Models::CameraConfigUpdate camera_config_model;
    
    try {
        auto json = nlohmann::json::parse(req.body);
        camera_config_model = Models::CameraConfigUpdate::fromJson(json);

    } catch (nlohmann::json::exception& e)
    {
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content(R"({"error":{"code":"INVALID_JSON","message":"Invalid camera configuration."}})", "application/json");
        return;

    }
    
    if (camera_config_model.height)
        app_config.rpicam_vid_wrapper_config.height = *(camera_config_model.height);

    if (camera_config_model.width)
        app_config.rpicam_vid_wrapper_config.width = *(camera_config_model.width);
    
    if (camera_config_model.fps)
        app_config.rpicam_vid_wrapper_config.framerate = *(camera_config_model.fps);
    
    app_.setConfig(app_config);
    
    res.status = httplib::StatusCode::OK_200;
}

void Api::onPostCameraStart(const httplib::Request& req, httplib::Response& res)
{
    auto task = [this](){
        app_.startRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostCameraStop(const httplib::Request& req, httplib::Response& res)
{
    auto task = [this](){
        app_.stopRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostCameraRestart(const httplib::Request& req, httplib::Response& res)
{
    // TODO: need some way to prevent multiple lifecycle actions from piling in the worker

    auto task = [this](){
        app_.stopRpiCamVid();
        app_.startRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}
