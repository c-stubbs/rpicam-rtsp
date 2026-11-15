#pragma once

#include <sys/types.h>
#include <optional>

#include "logger.h"
#include "rpicam_vid_wrapper_config.h"

class RpiCamVidWrapper {

    public:
        RpiCamVidWrapper(const RpiCamVidWrapperConfig& config);
        ~RpiCamVidWrapper();

        std::optional<pid_t> start(const std::optional<RpiCamVidWrapperConfig>& config = std::nullopt);
        void stop();
        bool isRunning();

    private:
        std::optional<pid_t> pid_;
        Logger log_;
        RpiCamVidWrapperConfig config_;
        
};
