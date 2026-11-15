# rpicam-vid wrapper
add_library(utils)

target_sources(utils
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.cpp)

target_include_directories(utils
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/utils)

target_link_libraries(utils
    PRIVATE
    logger)

