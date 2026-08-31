# rpicam-vid wrapper
add_library(rpicam_vid_wrapper)

target_sources(rpicam_vid_wrapper
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rpicam_vid_wrapper.cpp)

target_include_directories(rpicam_vid_wrapper
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/rpicam_vid_wrapper)

target_link_libraries(rpicam_vid_wrapper
    PUBLIC
    rpicam_vid_wrapper_config
    PRIVATE
    logger)

# rpicam-vid wrapper config
add_library(rpicam_vid_wrapper_config)

target_sources(rpicam_vid_wrapper_config
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rpicam_vid_wrapper_config.cpp)

target_include_directories(rpicam_vid_wrapper_config
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/rpicam_vid_wrapper)

target_link_libraries(rpicam_vid_wrapper_config
    PRIVATE
    logger
    PUBLIC
    tomlplusplus)
