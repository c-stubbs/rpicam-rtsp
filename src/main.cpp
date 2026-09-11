#include <exception>
#include <filesystem>
#include <toml++/toml.h>

#include "logger.h"
#include "app.h"

std::filesystem::path executable_dir()
{
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
}

int main(int argc, char *argv[])
{
    Logger log("main", "trace");

    std::filesystem::path sys_config_path{"/etc/rpicam-rtsp/config.toml"};
    std::filesystem::path local_config_path = executable_dir().parent_path() / "etc" / "rpicam-rtsp" / "config.toml";

    toml::table config_toml;

    if (std::filesystem::exists(sys_config_path))
    {
        config_toml = toml::parse_file(sys_config_path.string());
    }
    else if (std::filesystem::exists(local_config_path))
    {
        config_toml = toml::parse_file(local_config_path.string());
    }
    else
    {
        log.critical("Fatal error: config file not found");
        return 1;
    }

    AppConfig app_config = AppConfig::fromToml(config_toml);

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

