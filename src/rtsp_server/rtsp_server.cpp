#include "rtsp_server.h"
#include <iostream>

RTSPServer::RTSPServer(const RtspServerConfig& config) 
    : config_(config)
    , log_("rtsp_server", config.log_level)
{
}

RTSPServer::~RTSPServer()
{
    stop();
}

bool RTSPServer::start()
{
    if (isRunning())
    {
        return false;
    }

    loop_ = g_main_loop_new(nullptr, FALSE);

    if (!loop_)
    {
        log_.error("failed to create g_main_Loop");
        return false;
    }

    server_ = gst_rtsp_server_new();

    if (!server_)
    {
        log_.error("failed to create rtsp server");
        g_main_loop_unref(loop_);
        loop_ = nullptr;
        return false;
    }

    gst_rtsp_server_set_address(
            server_,
            address_.c_str());

    gst_rtsp_server_set_service(
            server_,
            std::to_string(config_.port).c_str());

    GstRTSPMountPoints* mounts = gst_rtsp_server_get_mount_points(server_);

    if (!mounts)
    {
        log_.error("failed to get rtsp mount points");

        g_object_unref(server_);
        server_ = nullptr;

        g_main_loop_unref(loop_);
        loop_ = nullptr;

        return false;
    }

    std::string pipeline =
        "( udpsrc port=" +
        std::to_string(config_.udp_port) +
        " "
        "caps=\"video/x-h264,stream-format=byte-stream,alignment=nal\" "
        "! rtph264pay name=pay0 pt=96 config-interval=1 mtu=1200 )";

       GstRTSPMediaFactory* factory =
        gst_rtsp_media_factory_new();

    if (!factory)
    {
        log_.error("failed to create rtsp media factory");

        g_object_unref(mounts);
        g_object_unref(server_);
        server_ = nullptr;

        g_main_loop_unref(loop_);
        loop_ = nullptr;

        return false;
    }

    gst_rtsp_media_factory_set_launch(
        factory,
        pipeline.c_str());

    gst_rtsp_mount_points_add_factory(
        mounts,
        config_.mount_point.c_str(),
        factory);

    g_object_unref(mounts);

    server_id_ =
        gst_rtsp_server_attach(server_, nullptr);

    if (server_id_ == 0)
    {
        log_.error("failed to attach rtsp server");
        g_object_unref(server_);
        server_ = nullptr;

        g_main_loop_unref(loop_);
        loop_ = nullptr;

        return false;
    }

    loop_thread_ = std::thread([this]()
            {
                g_main_loop_run(loop_);
            });

    log_.info("started");
    return true;
}

void RTSPServer::stop()
{
    if (!loop_)
    {
        return;
    }

    g_main_loop_quit(loop_);

    if (loop_thread_.joinable())
    {
        loop_thread_.join();
    }

    if (server_id_ != 0)
    {
        g_source_remove(server_id_);
        server_id_ = 0;
    }

    if (server_)
    {
        g_object_unref(server_);
        server_ = nullptr;
    }

    g_main_loop_unref(loop_);
    loop_ = nullptr;

    log_.info("stopped");
}

bool RTSPServer::isRunning()
{
    return server_ != nullptr &&
           server_id_ != 0 &&
           loop_ != nullptr &&
           g_main_loop_is_running(loop_);
}

