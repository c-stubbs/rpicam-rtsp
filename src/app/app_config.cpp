#include "app_config.h"

AppConfig AppConfig::fromToml(const toml::v3::ex::parse_result& config)
{
    return AppConfig{config["app"]["log_level"].value_or("info"),
                     RtspServerConfig::fromToml(config),
                     RpiCamVidWrapperConfig::fromToml(config)};
}

AppConfig::AppConfig(std::string log_level, RtspServerConfig rtsp_server_config, RpiCamVidWrapperConfig rpicam_vid_wrapper_config)
    : log_level(log_level)
    , rtsp_server_config(rtsp_server_config)
    , rpicam_vid_wrapper_config(rpicam_vid_wrapper_config)
{
    validate();
}

void AppConfig::validate() const
{
    // TODO
}
