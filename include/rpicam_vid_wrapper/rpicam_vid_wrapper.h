#include <sys/types.h>
#include <optional>

class RpiCamVidWrapper {

    public:
        ~RpiCamVidWrapper();

        std::optional<pid_t> start();
        void stop();
        bool isRunning();

    private:
        std::optional<pid_t> pid_;
        
};
