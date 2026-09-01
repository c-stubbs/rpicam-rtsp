#include <exception>
#include <toml++/toml.h>

#include "logger.h"
#include "app.h"

int main(int argc, char *argv[])
{
    Logger log("main", "trace");

    std::string config_path = std::string(CONFIG_DIR) + std::string("config.toml");

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

