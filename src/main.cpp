#include <exception>

#include "logger.h"
#include "app.h"

int main(int argc, char *argv[])
{
    Logger log("main", "trace");

    try {
        // TODO: read in toml and map appConfig object
        App app;
        return app.run(argc, argv);
    }
    catch (const std::exception& e)
    {
        log.critical("Fatal error: {}", e.what());
        return 1;
    }
}

