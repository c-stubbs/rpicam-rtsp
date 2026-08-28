#include "rtsp_server.h"
#include <iostream>

RTSPServer::RTSPServer()
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
        std::cerr << "Failed to create GMainLoop\n";
        return false;
    }

    server_ = gst_rtsp_server_new();

    if (!server_)
    {
        std::cerr << "Failed to create RTSP server\n";
        g_main_loop_unref(loop_);
        loop_ = nullptr;
        return false;
    }

    gst_rtsp_server_set_address(
            server_,
            address_.c_str());

    gst_rtsp_server_set_service(
            server_,
            std::to_string(port_).c_str());

    GstRTSPMountPoints* mounts = gst_rtsp_server_get_mount_points(server_);

    if (!mounts)
    {
        std::cerr << "Failed to get RTSP mount points\n";

        g_object_unref(server_);
        server_ = nullptr;

        g_main_loop_unref(loop_);
        loop_ = nullptr;

        return false;
    }

    std::string pipeline =
        "( udpsrc port=" +
        std::to_string(udp_port_) +
        " "
        "caps=\"video/x-h264,stream-format=byte-stream,alignment=nal\" "
        "! rtph264pay name=pay0 pt=96 config-interval=1 mtu=1200 )";

       GstRTSPMediaFactory* factory =
        gst_rtsp_media_factory_new();

    if (!factory)
    {
        std::cerr << "Failed to create RTSP media factory\n";

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
        mount_point_.c_str(),
        factory);

    g_object_unref(mounts);

    server_id_ =
        gst_rtsp_server_attach(server_, nullptr);

    if (server_id_ == 0)
    {
        std::cerr << "Failed to attach RTSP server\n";

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
}

bool RTSPServer::isRunning()
{
    return server_ != nullptr &&
           server_id_ != 0 &&
           loop_ != nullptr &&
           g_main_loop_is_running(loop_);
}

