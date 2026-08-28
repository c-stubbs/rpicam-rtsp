#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <glib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>

pid_t startCamera(
    int width,
    int height,
    int fps,
    int bitrate)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {

        std::vector<std::string> args = {
            "rpicam-vid",
            "-t", "0",
            "-v", "0",
            "--width", std::to_string(width),
            "--height", std::to_string(height),
            "--framerate", std::to_string(fps),
            "--bitrate", std::to_string(bitrate),
            "--inline",
            "-o", "udp://127.0.0.1:5000"
        };

        std::vector<char*> argv;

        for (auto& arg : args) {
            argv.push_back(arg.data());
        }

        argv.push_back(nullptr);

        execvp(argv[0], argv.data());

        perror("execvp");
        _exit(1);
    }

    return pid;
}

int main(int argc, char *argv[])
{

    pid_t cameraPid = startCamera(
        1920,       // width
        1080,       // height
        15,         // FPS
        1000000     // 4 Mbps
    );

    if (cameraPid < 0) {
        return 1;
    }
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);

    GstRTSPServer *server = gst_rtsp_server_new();

    gst_rtsp_server_set_address(server, "0.0.0.0");
    gst_rtsp_server_set_service(server, "8554");

    GstRTSPMountPoints *mounts =
        gst_rtsp_server_get_mount_points(server);

    const gchar *pipeline =
        "( udpsrc port=5000 "
        "caps=\"video/x-h264,stream-format=byte-stream,alignment=nal\" ! "
        "rtph264pay name=pay0 pt=96 config-interval=1 mtu=1200 )";

    GstRTSPMediaFactory *factory =
        gst_rtsp_media_factory_new();

    gst_rtsp_media_factory_set_launch(factory, pipeline);

    gst_rtsp_mount_points_add_factory(
        mounts,
        "/stream",
        factory
    );

    g_object_unref(mounts);

    if (gst_rtsp_server_attach(server, nullptr) == 0) {
        std::cerr << "Failed to attach RTSP server\n";
        return 1;
    }

    std::cout
        << "RTSP server listening at "
        << "rtsp://<PI-IP>:8554/stream\n";

    g_main_loop_run(loop);

    g_main_loop_unref(loop);
    g_object_unref(server);

    return 0;
}

