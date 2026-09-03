# api lib
add_library(api)

target_sources(api
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp)

target_include_directories(api
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/api
    PRIVATE
    ${CMAKE_SOURCE_DIR}/external/cpp-httplib)

target_link_libraries(api
    PUBLIC
    api_config
    PRIVATE
    logger)

# rpicam-vid wrapper config
add_library(api_config)

target_sources(api_config
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/api_config.cpp)

target_include_directories(api_config
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/api)

target_link_libraries(api_config
    PRIVATE
    logger
    PUBLIC
    tomlplusplus)
