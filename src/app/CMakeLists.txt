# app
add_library(app)

target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp)

target_include_directories(app
    PUBLIC
    ${GST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/app)

target_link_libraries(app
    PUBLIC
    ${GST_LIBRARIES}
    app_config
    PRIVATE
    logger
    rtsp_server
    rpicam_vid_wrapper)

# rtsp server config
add_library(app_config)

target_sources(app_config
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/app_config.cpp)

target_include_directories(app_config
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/app)

target_link_libraries(app_config
    PRIVATE
    logger
    PUBLIC
    tomlplusplus
    rpicam_vid_wrapper_config
    rtsp_server_config)
