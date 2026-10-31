
#include <iostream>

#include "cli_args.h"

void CliArgs::parse(int& argc, char *argv[])
{
    cxxopts::Options options("rpicam-rtsp", "A GStreamer-based RTSP wrapper around rpicam-vid");

    help_.addTo(options);

    app_log_level_.addTo(options);
    rtsp_server_log_level_.addTo(options);
    rtsp_port_.addTo(options);
    udp_port_.addTo(options);
    mount_point_.addTo(options);
    pipeline_.addTo(options);
    run_rtsp_server_.addTo(options);
    rpicam_vid_log_level_.addTo(options);
    video_width_.addTo(options);
    video_height_.addTo(options);
    framerate_.addTo(options);
    bitrate_.addTo(options);
    run_rpicam_vid_.addTo(options);

    auto result = options.parse(argc, argv);

    help_.getFrom(result);
    
    if (help_.value)
        std::cout << options.help() << std::endl; 

    app_log_level_.getFrom(result);
    rtsp_server_log_level_.getFrom(result);
    rtsp_port_.getFrom(result);
    udp_port_.getFrom(result);
    mount_point_.getFrom(result);
    pipeline_.getFrom(result);
    run_rtsp_server_.getFrom(result);
    rpicam_vid_log_level_.getFrom(result);
    video_width_.getFrom(result);
    video_height_.getFrom(result);
    framerate_.getFrom(result);
    bitrate_.getFrom(result);
    run_rpicam_vid_.getFrom(result);
}

bool CliArgs::hasHelp() const
{
    return help_.value.has_value();
}

void CliArgs::applyOverridesTo(AppConfig& app_config) const
{
    if (app_log_level_.value)
        app_config.log_level = *(app_log_level_.value);

    if (rtsp_server_log_level_.value)
        app_config.rtsp_server_config.log_level = *(rtsp_server_log_level_.value);

    if (rtsp_port_.value)
        app_config.rtsp_server_config.port = *(rtsp_port_.value);

    if (udp_port_.value)
    {
        app_config.rtsp_server_config.udp_port = *(udp_port_.value);
        app_config.rpicam_vid_wrapper_config.port = *(udp_port_.value);
    }

    if (mount_point_.value)
        app_config.rtsp_server_config.mount_point = *(mount_point_.value);

    if (pipeline_.value)
        app_config.rtsp_server_config.pipeline = *(pipeline_.value);

    if (run_rtsp_server_.value)
        app_config.rtsp_server_config.run = *(run_rtsp_server_.value);

    if (rpicam_vid_log_level_.value)
        app_config.rpicam_vid_wrapper_config.log_level = *(rpicam_vid_log_level_.value);

    if (video_width_.value)
        app_config.rpicam_vid_wrapper_config.width = *(video_width_.value);

    if (video_height_.value)
        app_config.rpicam_vid_wrapper_config.height = *(video_height_.value);

    if (framerate_.value)
        app_config.rpicam_vid_wrapper_config.framerate = *(framerate_.value);

    if (bitrate_.value)
        app_config.rpicam_vid_wrapper_config.bitrate = *(bitrate_.value);

    if (run_rpicam_vid_.value)
        app_config.rpicam_vid_wrapper_config.run = *(run_rpicam_vid_.value);

}
