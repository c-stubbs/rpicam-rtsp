#include <optional>

class CliArgs {

    public:

        CliArgs(int& argc, char *argv[]);

        void applyOverrides(/* AppConfig& app_config */);
    
    private:

        std::optional<std::string> app_log_level_;

        std::optional<std::string> rtsp_server_log_level_;
        std::optional<int> rtsp_port_;
        std::optional<int> udp_port_;
        std::optional<std::string> mount_point_;
        std::optional<std::string> pipeline_;
        std::optional<bool> run_rtsp_server_;

        std::optional<std::string> rpicam_vid_log_level_;
        std::optional<int> video_width_;
        std::optional<int> video_height_;
        std::optional<int> framerate_;
        std::optional<int> bitrate_;
        std::optional<bool> run_rpicam_vid_;

};
