#!/bin/bash

sudo apt update
sudo apt install libglib2.0-dev \
        libgstreamer1.0-dev \
        gstreamer1.0-plugins-base \
        gstreamer1.0-plugins-good \
        gstreamer1.0-plugins-ugly \
        gstreamer1.0-plugins-bad \
        gstreamer1.0-libav \
        libgstrtspserver-1.0-dev \
        gir1.2-gstreamer-1.0 \
        gir1.2-gst-rtsp-server-1.0 \
        libspdlog-dev \
        libtomlplusplus-dev \
        rpicam-apps

