cmake_minimum_required(VERSION 3.28)
project(rpicam-rtsp)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

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
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/rpicam-rtsp
)

# CPack for .deb file
file(WRITE ${CMAKE_BINARY_DIR}/conffiles "/etc/rpicam-rtsp/config.toml\n")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "rpicam-rtsp")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A GStreamer-based RTSP wrapper around rpicam-vid")
set(CPACK_DEBIAN_PACKAGE_MANTAINER "Chandler Stubbs")
set(CPACK_PACKAGE_CONTACT "todo@gmail.com")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/conffiles")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "rpicam-apps, libgstreamer1.0-0, libgstrtspserver-1.0-0, libfmt10")

include(CPack)
