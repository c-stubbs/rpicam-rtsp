# rtsp server
add_library(rtsp_server)

target_sources(rtsp_server
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rtsp_server.cpp)

target_include_directories(rtsp_server
    PUBLIC
    ${GST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/rtsp_server)

target_link_libraries(rtsp_server
    PUBLIC
    ${GST_LIBRARIES}
    rtsp_server_config
    PRIVATE
    logger)

# rtsp server config
add_library(rtsp_server_config)

target_sources(rtsp_server_config
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rtsp_server_config.cpp)

target_include_directories(rtsp_server_config
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/rtsp_server)

target_link_libraries(rtsp_server_config
    PRIVATE
    logger
    PUBLIC
    tomlplusplus)
