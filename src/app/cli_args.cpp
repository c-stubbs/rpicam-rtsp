#include "cxxopts.hpp"

#include "cli_args.h"

CliArgs::CliArgs(int& argc, char *argv[])
{
    cxxopts::Options options("rpicam-rtsp", "A GStreamer-based RTSP wrapper around rpicam-vid");

    options.add_options()
        ("log-level", "Log level of the app", cxxopts::value<std::string>())
        ("rtsp-server-log-level", "Log level of RTSP specific behavior", cxxopts::value<std::string>())
        ("rtsp-port", "Port for the RTSP server", cxxopts::value<int>())
        ("udp-port", "Port used to to bridge rpicam-vid to the RTSP server", cxxopts::value<int>())
        ("mount-point", "Mount point of the rtsp stream e.g., /stream", cxxopts::value<std::string>())
        ("pipeline", "Overrides the pipeline used by gst rtsp server", cxxopts::value<std::string>())
        ("run-rtsp", "Enable the RTSP server")
        ("rpicam-vid-log-level", "Log level of rpicam-vid specific behavior", cxxopts::value<std::string>())
        ("width", "Width of the video", cxxopts::value<int>())
        ("height", "Height of the video", cxxopts::value<int>())
        ("framerate", "Target framerate of the stream", cxxopts::value<int>())
        ("bitrate", "Bitrate of the rpicam-vid udp stream", cxxopts::value<int>())
        ("run-rpicam-vid", "Enable rpicam-vid");


    auto result = options.parse(argc, argv);

    if (result.contains("log-level"))
        app_log_level_ = result["log-level"].as<std::string>();

    if (result.contains("rtsp-server-log-level"))
        rtsp_server_log_level_ = result["rtsp-server-log-level"].as<std::string>();



}

void CliArgs::applyOverrides()
{

}
