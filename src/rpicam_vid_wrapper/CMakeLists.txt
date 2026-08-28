# Add main executable
add_library(rpicam_vid_wrapper)

target_sources(rpicam_vid_wrapper
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rpicam_vid_wrapper.cpp)

target_include_directories(rpicam_vid_wrapper
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/rpicam_vid_wrapper)
