#pragma once

#include <sys/types.h>
#include <optional>
#include "logger.h"

class RpiCamVidWrapper {

    public:
        RpiCamVidWrapper();
        ~RpiCamVidWrapper();

        std::optional<pid_t> start();
        void stop();
        bool isRunning();

    private:
        std::optional<pid_t> pid_;
        Logger log_;
        
};
