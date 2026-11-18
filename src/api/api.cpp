#include "api.h"

#include "app.h"
#include "httplib.h"
#include "models.h"
#include "thread_pool.h"
#include "string_utils.h"

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
    log_.trace("start() called");

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

    server.Get("/api/v1/rtsp/config",
                [this](const httplib::Request& req, httplib::Response& res)
                {
                    this->onGetRtspConfig(req, res);
                });

    server_thread_ = std::jthread([this](){
        server.listen("0.0.0.0", 8080);
    });

    log_.info("started");

    return true;
}

void Api::stop()
{
    log_.trace("stop() called");

    server.stop();
 
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }

    log_.info("stopped");
}

void Api::onGetMarco(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onGetMarco() called");

    res.set_content("Polo!", "text/plain");
}

void Api::onGetCameraConfig(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onGetCameraConfig() called");

    auto camera_config = app_.getConfig().rpicam_vid_wrapper_config;

    Models::CameraConfig config;
    config.height = camera_config.height;
    config.width = camera_config.width;
    config.framerate = camera_config.framerate;
    config.bitrate = camera_config.bitrate;
    config.port = camera_config.port;
    config.log_level = camera_config.log_level;
    config.raw_args = StringUtils::vectorToString(camera_config.raw_args);

    res.set_content(config.dump(), "application/json");
}

void Api::onPatchCameraConfig(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPatchCameraConfig() called");

    auto app_config = app_.getConfig();
    Models::CameraConfigUpdate camera_config_model;
    
    try {
        auto json = nlohmann::json::parse(req.body);
        camera_config_model = Models::CameraConfigUpdate::fromJson(json);

        log_.debug("received camera configuration: {}", json.dump());

    } catch (nlohmann::json::exception& e)
    {
        log_.error("received invalid camera configuration: {}", e.what());

        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content(R"({"error":{"code":"INVALID_JSON","message":"Invalid camera configuration."}})", "application/json");
        return;

    }
    
    if (camera_config_model.height)
        app_config.rpicam_vid_wrapper_config.height = *(camera_config_model.height);

    if (camera_config_model.width)
        app_config.rpicam_vid_wrapper_config.width = *(camera_config_model.width);
    
    if (camera_config_model.framerate)
        app_config.rpicam_vid_wrapper_config.framerate = *(camera_config_model.framerate);

    if (camera_config_model.bitrate)
        app_config.rpicam_vid_wrapper_config.bitrate = *(camera_config_model.bitrate);

    if (camera_config_model.port)
        app_config.rpicam_vid_wrapper_config.port = *(camera_config_model.port);

    if (camera_config_model.log_level)
        app_config.rpicam_vid_wrapper_config.log_level = *(camera_config_model.log_level);

    if (camera_config_model.raw_args)
        app_config.rpicam_vid_wrapper_config.raw_args = StringUtils::splitString(*(camera_config_model.raw_args), ' ');
    
    app_.setConfig(app_config);
   
    res.status = httplib::StatusCode::OK_200;
}

void Api::onPostCameraStart(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPostCameraStart() called");

    auto task = [this](){
        app_.startRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostCameraStop(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPostCameraStop() called");

    auto task = [this](){
        app_.stopRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostCameraRestart(const httplib::Request& req, httplib::Response& res)
{
    // TODO: need some way to prevent multiple lifecycle actions from piling in the worker

    log_.trace("onPostCameraRestart() called");

    auto task = [this](){
        app_.stopRpiCamVid();
        app_.startRpiCamVid();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onGetRtspConfig(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onGetRtspConfig() called");

    auto rtsp_config = app_.getConfig().rtsp_server_config;

    Models::RtspConfig config;
    config.port = rtsp_config.port;
    config.udp_port = rtsp_config.udp_port;
    config.mount_point = rtsp_config.mount_point;
    config.pipeline = rtsp_config.pipeline;
    config.log_level = rtsp_config.log_level;

    res.set_content(config.dump(), "application/json");
}

void Api::onPatchRtspConfig(const httplib::Request& req, httplib::Response& res)
{
    // TODO
}

void Api::onPostRtspStart(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPostRtspStart() called");

    auto task = [this](){
        app_.startRtspServer();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostRtspStop(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPostRtspStop() called");

    auto task = [this](){
        app_.stopRtspServer();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}

void Api::onPostRtspRestart(const httplib::Request& req, httplib::Response& res)
{
    log_.trace("onPostRtspRestart() called");

    auto task = [this](){
        app_.stopRtspServer();
        app_.startRtspServer();
    };

    thread_pool_.queue(std::move(task));

    res.status = httplib::StatusCode::Accepted_202;
}
