cmake_minimum_required(VERSION 3.28)
project(rpicam-rtsp)

include(GNUInstallDirs)

set(CONFIG_DIR
    "${CMAKE_INSTALL_PREFIX}/config/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_COMPILER=clang)
set(CMAKE_CXX_COMPILER=clang++)

# find requried packages
find_package(PkgConfig REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(tomlplusplus REQUIRED)
pkg_check_modules(GST REQUIRED 
    gstreamer-1.0
    gstreamer-rtsp-server-1.0)

# Add logger library
add_library(logger INTERFACE)

target_include_directories(logger 
    INTERFACE 
    include/logger)

target_link_libraries(logger
    INTERFACE
    fmt)

add_subdirectory(src/rpicam_vid_wrapper)
add_subdirectory(src/rtsp_server)
add_subdirectory(src/app)
add_subdirectory(src/api)

# Add main executable
add_executable(${PROJECT_NAME} 
    src/main.cpp)

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE
    CONFIG_DIR="${CONFIG_DIR}")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    rtsp_server
    rpicam_vid_wrapper
    app
    logger
    api
    tomlplusplus)

# Install bins
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install configs
install(
    DIRECTORY config/
    DESTINATION config
)
