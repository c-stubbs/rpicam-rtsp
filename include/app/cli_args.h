#include "cxxopts.hpp"

#include "app_config.h"

#include <optional>

class CliArgs {

    public:

        void parse(int& argc, char *argv[]);

        void applyOverridesTo(AppConfig& app_config) const;
        
        bool hasHelp() const;

    private:

        template <typename T>
        struct Option {

            Option(std::string name, std::string description) : name(name), description(description) {};
            Option(std::string short_name, std::string name, std::string description) : short_name(short_name), name(name), description(description) {};
            
            std::optional<T> value;
            std::string name;
            std::string short_name;
            std::string description;

            void getFrom(const cxxopts::ParseResult& parse_result)
            {
                if (parse_result.count(name))
                    value = parse_result[name].as<T>();
            };

            void addTo(cxxopts::Options& options) const
            {
                if (short_name.empty())
                    options.add_options()(name, description, cxxopts::value<T>());

                else
                    options.add_options()(short_name + "," + name, description, cxxopts::value<T>());
            }
        };

        Option<std::string> app_log_level_{"log-level", "Default log level"};

        Option<std::string> rtsp_server_log_level_{"rtsp-log-level", "Log level of RTSP specific behavior"};
        Option<int> rtsp_port_{"rtsp-port","Port for the RTSP server"};
        Option<int> udp_port_{"udp-port", "Port used to bridge rpicam-vid to the RTSP server"};
        Option<std::string> mount_point_{"mount-point","Mount point of the RTSP stream e.g., /stream"};
        Option<std::string> pipeline_{"pipeline", "Overrides the pipeline used by the RTSP server"};
        Option<bool> run_rtsp_server_{"run-rtsp", "Enable the RTSP server"};

        Option<std::string> rpicam_vid_log_level_{"rpicam-vid-log-level", "Log level of rpicam-vid specific behavior"};
        Option<int> video_width_{"width", "Width of the video stream"};
        Option<int> video_height_{"height", "Height of the video stream"};
        Option<int> framerate_{"fps", "Target framerate of the video stream"};
        Option<int> bitrate_{"bitrate", "Bitrate of the video stream"};
        Option<bool> run_rpicam_vid_{"run-rpicam-vid", "Enable rpicam-vid"};

        Option<bool> help_{"h", "help", "Display help"};
};
