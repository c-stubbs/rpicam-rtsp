# rpicam-rtsp

A lightweight C++ RTSP server for Raspberry Pi cameras using [`rpicam-vid`](https://github.com/raspberrypi/rpicam-apps) and GStreamer.

`rpicam-rtsp` wraps `rpicam-vid` and exposes its H.264 output as an RTSP stream, providing a simple way to get a Raspberry Pi camera onto an IP network without requiring `rpicam-vid` to implement RTSP itself.

## Overview

The project uses `rpicam-vid` to capture and encode video, sends the encoded H.264 stream over a local UDP socket, and uses GStreamer to package that stream into RTSP.

```text
┌──────────────────┐
│ Raspberry Pi     │
│                  │
│  Camera          │
│     │            │
│     ▼            │
│ rpicam-vid       │
│  H.264 encode    │
│     │            │
│     │ UDP         │
│     ▼            │
│ GStreamer        │
│ RTSP Server      │
└─────┬────────────┘
      │
      │ RTSP
      ▼
┌──────────────────┐
│ RTSP Client      │
│                  │
│ VLC / ffplay /   │
│ MediaMTX / etc.  │
└──────────────────┘
```

The goal is to keep the camera capture and RTSP serving components relatively independent. `rpicam-vid` handles camera configuration and H.264 encoding, while GStreamer handles the network-facing RTSP server.

## Features

* Uses Raspberry Pi's `rpicam-vid` camera stack
* H.264 hardware-accelerated encoding
* Lightweight C++ wrapper around `rpicam-vid`
* GStreamer-based RTSP server
* Suitable for use with MediaMTX and WebRTC viewers

## Requirements

### Hardware

* Raspberry Pi with a supported camera
* Raspberry Pi OS
* Network connection

The project is intended for Raspberry Pi hardware, including the Raspberry Pi Zero 2 W.

### Software

* C++ compiler with C++17 support
* CMake
* GStreamer
* GStreamer RTSP Server
* `rpicam-vid`

On Raspberry Pi OS, the required dependencies can be installed with the provided setup script.

GStreamer development packages are also required to build the project.

## Building

### On a Raspberry Pi

Clone the repository and run the setup and build scripts:

```bash
git clone https://github.com/c-stubbs/rpicam-rtsp.git
cd rpicam-rtsp
./scripts/setup.sh
./scripts/build.sh
```

The resulting executable is installed to:

```text
install/bin/rpicam-rtsp
```

It can be run directly on the Raspberry Pi.

### Cross-Compiling from a Separate Machine

The project can also be cross-compiled for the Raspberry Pi from an x86-64 Linux machine. The build uses a sysroot generated from the Raspberry Pi so that the compiler and linker can use the Pi's libraries and headers.

#### 1. Clone the repository

Clone the repository on both the Raspberry Pi and the development machine:

```bash
git clone https://github.com/c-stubbs/rpicam-rtsp.git
cd rpicam-rtsp
```

#### 2. Set up the Raspberry Pi

On the Raspberry Pi, run:

```bash
./scripts/setup.sh
```

This installs the dependencies required to build and run `rpicam-rtsp`.

#### 3. Generate the sysroot

On the development machine, generate a sysroot from the Raspberry Pi:

```bash
./scripts/generate-sysroot.sh
```

The sysroot contains the Raspberry Pi's headers, libraries, and CMake/pkg-config files needed for cross-compilation.

#### 4. Configure the cross-compilation environment

Set the environment variables required by the cross-compilation toolchain:

```bash
export RPI_SYSROOT=...
```

See the toolchain configuration in `cmake/toolchains/raspberry_pi.cmake` for the required configuration.

#### 5. Build

Run the cross-compilation build:

```bash
./scripts/xbuild.sh
```

The resulting executable is installed to:

```text
xinstall/bin/rpicam-rtsp
```

#### 6. Copy the executable to the Raspberry Pi

Copy the executable to the Raspberry Pi:

```bash
scp xinstall/bin/rpicam-rtsp <user>@<raspberry-pi>:/path/to/destination/
```

The executable can then be run on the Raspberry Pi.


## Usage

Start the RTSP server:

```bash
./install/bin/rpicam-rtsp
```

By default, the server exposes the camera stream at:

```text
rtsp://<raspberry-pi-ip>:8554/stream
```

For example:

```bash
ffplay rtsp://192.168.1.100:8554/stream
```

You can also open the URL in VLC or use it as an input to another video-processing application.

## Architecture

The application consists of two primary components.

### `rpicam-vid`

`rpicam-vid` is launched as a child process by the C++ application.

Rather than writing the camera capture and encoding pipeline directly against the Raspberry Pi camera API, `rpicam-rtsp` delegates those responsibilities to the Raspberry Pi camera tools.

The encoded H.264 stream is sent to a local UDP socket.

```text
rpicam-vid
    │
    │ H.264 over UDP
    ▼
localhost:<port>
```

### GStreamer RTSP server

GStreamer receives the H.264 packets from the UDP socket and exposes them through an RTSP endpoint.

Conceptually, the pipeline looks like:

```text
UDP
 │
 ▼
udpsrc
 │
 ▼
H.264 parser
 │
 ▼
RTP payloader
 │
 ▼
RTSP server
```

This separation makes it possible to configure the Raspberry Pi camera using the existing `rpicam-vid` command-line interface while keeping the RTSP server implementation in C++.

## Why UDP?

The communication between `rpicam-vid` and the RTSP server happens locally on the Raspberry Pi.

UDP is used as a simple transport boundary between the two processes:

```text
rpicam-vid ──UDP──> GStreamer
```

Since the transport stays on localhost, packet loss and network congestion are not significant concerns. The separation also means either side can be developed and tested independently.

## MediaMTX

One intended use is to publish the stream to [MediaMTX](https://github.com/bluenviron/mediamtx), which can then make the camera available through multiple protocols, including WebRTC.

For example:

```text
Raspberry Pi
┌─────────────────────┐
│ Camera              │
│   ↓                 │
│ rpicam-vid          │
│   ↓ UDP             │
│ GStreamer RTSP      │
└─────────┬───────────┘
          │ RTSP
          ▼
┌─────────────────────┐
│ MediaMTX            │
│                     │
│ RTSP ───────────────┼──► RTSP clients
│ WebRTC ─────────────┼──► Web browsers
└─────────────────────┘
```

This allows the Raspberry Pi to focus on camera capture and encoding while a more powerful machine handles additional processing or protocol conversion.

## Project Status

This project is under active development.

## License

See: [License](LICENSE)
