#pragma once

#include <toml++/toml.h>

#include "rpicam_vid_wrapper_config.h"
#include "rtsp_server_config.h"

class AppConfig {

    public:
        static AppConfig fromToml(const toml::v3::ex::parse_result& config);
        AppConfig(std::string log_level, 
                  RtspServerConfig rtsp_server_config, 
                  RpiCamVidWrapperConfig rpicam_vid_wrapper_config);

        std::string log_level;

        RtspServerConfig rtsp_server_config;
        RpiCamVidWrapperConfig rpicam_vid_wrapper_config;

        void validate() const;

    private:

};
