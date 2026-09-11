#include <exception>
#include <string_view>
#include <toml++/toml.h>

#include "logger.h"
#include "app.h"

int main(int argc, char *argv[])
{
    Logger log("main", "trace");

    constexpr std::string_view config_path = "/usr/etc/rpicam-rtsp/config.toml";

    auto config = toml::parse_file(config_path);
    AppConfig app_config = AppConfig::fromToml(config);

    try {
        App app(app_config);
        return app.run(argc, argv);
    }
    catch (const std::exception& e)
    {
        log.critical("Fatal error: {}", e.what());
        return 1;
    }
}

