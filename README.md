# Amazon Kinesis Video Streams Media Interface

[![file](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml)
[![v4l2](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml)
[![Gstreamer](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/Gstreamer.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/gstreamer.yml)
[![QCS610](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/QCS610/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/qualcomm-qcs610.yml)
[![ingenic-t31](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/ingenic-t31.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/ingenic-t31.yml)
[![fullhan-fh8626v100](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/fullhan-fh8626v100.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/fullhan-fh8626v100.yml)
[![anyka-ak3918](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/anyka-ak3918.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/anyka-ak3918.yml)
[![amlogic-c302](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/amlogic-c302.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/amlogic-c302.yml)
[![eeasy-sv82x](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/eeasy-sv82x.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/eeasy-sv82x.yml)

- [Amazon Kinesis Video Streams Media Interface](#amazon-kinesis-video-streams-media-interface)
  - [Introduction](#introduction)
  - [Supported Boards](#supported-boards)
  - [Getting started with out-of-box KVS WebRTC sample](#getting-started-with-out-of-box-kvs-webrtc-sample)
  - [Getting started with out-of-box KVS Producer sample](#getting-started-with-out-of-box-kvs-producer-sample)
  - [Getting started with out-of-box save frame sample](#getting-started-with-out-of-box-save-frame-sample)
  - [Cross compile FILE board](#cross-compile-file-board)
  - [Platform Implementation Guide](#platform-implementation-guide)
  - [Documentations](#documentations)
  - [Contributing](#contributing)
  - [Code Of Conduct](#code-of-conduct)
  - [License](#license)

## Introduction

**Amazon Kinesis Video Streams Media Interface** provides abstract interfaces for board specific media APIs. This repository also contains boards sensors/encoder implementations for Amazon Kinesis Video Streams Producer and WebRTC with out-of-box samples.

## Supported Boards

| Name               | Description                                                                 | CMake Parameter                      | Status                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| ------------------ | --------------------------------------------------------------------------- | ------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| FILE               | Dummy boards that can capture from [sample frames](resources/frames/)       | `-DBOARD=FILE`                       | [![file](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml)                                                                                                                                                                                                                                    |
| x86/x64            | Capture from V4L2 device or capture from [sample frames](resources/frames/) | `-DBOARD=V4L2`<br> or `-DBOARD=FILE` | [![v4l2](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml)[![file](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml) |
| Raspberry Pi       | Capture from V4L2 device or capture from [sample frames](resources/frames/) | `-DBOARD=V4L2`<br> or `-DBOARD=FILE` | [![v4l2](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/v4l2.yml)[![file](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/file.yml) |
| Gstreamer          | Capture from Gstreamer pipeline                                             | `-DBOARD=GSTREAMER`                  | [![gstreamer](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/gstreamer.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/gstreamer.yml)                                                                                                                                                                                                                     |
| Qualcomm QCS610    | IPC SoC designed by Qualcomm                                                | `-DBOARD=QCS610`                     | [![qualcomm-qcs610](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/qualcomm-qcs610.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/qualcomm-qcs610.yml)                                                                                                                                                                                                   |
| Ingenic T31        | IPC SoC designed by Ingenic                                                 | `-DBOARD=T31`                        | [![ingenic-t31](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/ingenic-t31.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/ingenic-t31.yml)                                                                                                                                                                                                               |
| Fullhan FH8626V100 | IPC SoC designed by Fullhan                                                 | `-DBOARD=FH8626V100`                 | [![fullhan-fh8626v100](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/fullhan-fh8626v100.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/fullhan-fh8626v100.yml)                                                                                                                                                                                          |
| Anyka AK3918       | IPC SoC designed by Anyka                                                   | `-DBOARD=AK3918`                     | [![anyka-ak3918](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/anyka-ak3918.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/anyka-ak3918.yml)                                                                                                                                                                                                            |
| Amlogic C302       | IPC SoC designed by Amlogic                                                 | `-DBOARD=C302`                       | [![amlogic-c302](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/amlogic-c302.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/amlogic-c302.yml)                                                                                                                                                                                                            |
| Eeasy SV82x        | IPC SoC designed by Eeasy                                                   | `-DBOARD=SV82x`                      | [![eeasy-sv82x](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/eeasy-sv82x.yml/badge.svg)](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/actions/workflows/eeasy-sv82x.yml)                                                                                                                                                                                                               |


## Getting started with out-of-box KVS WebRTC sample

1. Prepare the AWS credentials
   - If you want to use AWS access key id/access key secrets, you can generate AWS credentials by AWS IAM console.
   - Alternatively if you want to use AWS IoT Certificate, you can generate AWS credentials by using this [script](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/master/scripts/generate-iot-credential.sh)
2. Clone the code:
```bash
git clone https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface.git
```
3. Copy SoC SDK dependencies into *3rdparty/$BOARD/*. See [3rdparty/README.md](3rdparty/README.md).
4. Build the code:
```bash
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build; cmake .. -DBOARD=${YOUR_BOARD_NAME}
make
```
5. Upload built artifacts(i.e kvswebrtcmaster-static) at *amazon-kinesis-video-streams-media-interface/build/samples/webrtc/* to your board.
6. Setup AWS credentials in environment variables. 
    - If you want to use AWS access key id/access key secrets, use following commands to setup environment variables:
    ```bash
    export AWS_KVS_LOG_LEVEL=2
    export AWS_DEFAULT_REGION=us-east-1
    export AWS_KVS_CACERT_PATH=rootca.pem
    export AWS_ACCESS_KEY_ID=xxxxxxxxxxxxxxxxxxxx
    export AWS_SECRET_ACCESS_KEY=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    ```
    > Replace AWS_DEFAULT_REGION, AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY with your own credentials

    > You can use Root CA in [resources/certs/rootca.pem](resources/certs/rootca.pem), or you can download it from [Amazon Trust Services](https://www.amazontrust.com/repository/SFSRootCAG2.pem)
    - Alternatively if you want to use AWS IoT Certificate, turn on `IOT_CORE_ENABLE_CREDENTIALS` in [Samples.h](https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface/blob/main/samples/webrtc/source/Samples.h#L67) and using following commands to setup environment variables:
    ```bash
    export AWS_KVS_LOG_LEVEL=2
    export AWS_DEFAULT_REGION=us-east-1
    export AWS_KVS_CACERT_PATH=rootca.pem
    export AWS_IOT_CORE_THING_NAME=your_camera_name
    export AWS_IOT_CORE_CREDENTIAL_ENDPOINT=xxxxxxxxxxxxxx.credentials.iot.us-east-1.amazonaws.com
    export AWS_IOT_CORE_CERT=your_camera_certificate.pem
    export AWS_IOT_CORE_PRIVATE_KEY=your_camera_private.key
    export AWS_IOT_CORE_ROLE_ALIAS=your_camera_role_alias
    ```
    > You can use Root CA in [resources/certs/rootca.pem](resources/certs/rootca.pem), or you can download it from [Amazon Trust Services](https://www.amazontrust.com/repository/SFSRootCAG2.pem)
7. Make sure your the system time on your board has been synchronized. You may set it manually or use ntp client.
8. If you are using access key id/access key secret, execute sample with channel name as parameter: `./kvswebrtcmaster-static your_channel_name`. If you are using AWS IoT Certificate, execute sample on your board directly: `./kvswebrtcmaster-static`. 
9. Check WebRTC live stream via AWS console or [AWS WebRTC test page](https://d3etpwtx4wgido.cloudfront.net/)
    > Browser should work as viewer mode during test.

## Getting started with out-of-box KVS Producer sample

1. Clone the code:
```bash
git clone https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface.git
```
2. Copy SoC SDK dependencies into *3rdparty/$BOARD/*. See [3rdparty/README.md](3rdparty/README.md).
3. Build the code:
```bash
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build; cmake .. -DBUILD_WEBRTC_SAMPLES=OFF -DBUILD_KVS_SAMPLES=ON -DBOARD=${YOUR_BOARD_NAME}
make
```
4. Upload built artifacts(i.e kvsproducer-static) at *amazon-kinesis-video-streams-media-interface/build/samples/kvs/* to your board.
5. Setup AWS credentials.
  - If you want to use AWS access key id/access key secrets, use following commands to setup environment variables:
  ```bash
  export AWS_ACCESS_KEY_ID=xxxxxxxxxxxxxxxxxxxx
  export AWS_SECRET_ACCESS_KEY=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  export AWS_DEFAULT_REGION=us-east-1
  ```
  - Alternatively if you want to use AWS IoT Certificate, turn on `ENABLE_IOT_CREDENTIAL` in [samples/kvsproducer/source/sample_config.h](samples/kvsproducer/source/sample_config.h) and using following commands to setup environment variables:
  ```c
  #define ENABLE_IOT_CREDENTIAL           1
  ```
  ```bash
  export AWS_KVS_LOG_LEVEL=2
  export AWS_DEFAULT_REGION=us-east-1
  export AWS_KVS_CACERT_PATH=rootca.pem
  export AWS_IOT_CORE_THING_NAME=your_camera_name
  export AWS_IOT_CORE_CREDENTIAL_ENDPOINT=xxxxxxxxxxxxxx.credentials.iot.us-east-1.amazonaws.com
  export AWS_IOT_CORE_CERT=your_camera_certificate.pem
  export AWS_IOT_CORE_PRIVATE_KEY=your_camera_private.key
  export AWS_IOT_CORE_ROLE_ALIAS=your_camera_role_alias
  ```
6. Make sure your the system time on your board has been synchronized. You may set it manually or use ntp client.
7. Execute sample on your board: `./kvsproducer-static $YOUR_STREAM_NAME`
8. Check DASH live stream via [AWS KVS test page](https://aws-samples.github.io/amazon-kinesis-video-streams-media-viewer/)
9. Download video clips via AWS console.

> In current stage, browser doesn't support G.711 via HLS/DASH. To verify audio content in G.711 formats, user must download video clips.

## Getting started with out-of-box save frame sample

1. Clone the code:
```
git clone https://github.com/aws-samples/amazon-kinesis-video-streams-media-interface.git
```
2. Copy SoC SDK dependencies into *3rdparty/$BOARD/*. See [3rdparty/README.md](3rdparty/README.md).
3. Build the code:
```bash
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build; cmake .. -DBUILD_WEBRTC_SAMPLES=OFF -DBUILD_SAVE_FRAME_SAMPLES=ON -DBOARD=${YOUR_BOARD_NAME}
make
```
4. Execute sample on your board: `./saveframe-static $FILE_NAME`

## Cross compile FILE board

To cross compile `FILE` board sample, user need to setup `CC`, `CXX`, `USE_MUCLIBC` and `BOARD_DESTINATION_PLATFORM`.

For ARM boards with uclibc:
```bash
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build
cmake .. -DBOARD=FILE -DUSE_MUCLIBC=ON -DBOARD_DESTINATION_PLATFORM=arm-unknown-linux-uclibc
```
> If board is using glibc, use cmake command `cmake .. -DBOARD=FILE -DUSE_MUCLIBC=OFF -DBOARD_DESTINATION_PLATFORM=arm-unknown-linux-gnu` instead.

For MIPS boards with uclibc:
```bash
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build
cmake .. -DBOARD=FILE -DUSE_MUCLIBC=ON -DBOARD_DESTINATION_PLATFORM=mips-unknown-linux-uclibc
```
> If board is using glibc, use cmake command `cmake .. -DBOARD=FILE -DUSE_MUCLIBC=OFF -DBOARD_DESTINATION_PLATFORM=mips-unknown-linux-gnu` instead.

## Platform Implementation Guide

To adapt other platforms SDKs with **Amazon Kinesis Video Streams Media Interface**, you need to implement interfaces in *include/com/amazonaws/kinesis/video/capturer/VideoCapturer.h*, *include/com/amazonaws/kinesis/video/capturer/AudioCapturer.h* and *include/com/amazonaws/kinesis/video/player/AudioPlayer.h*:

- [VideoCapturer.h](include/com/amazonaws/kinesis/video/capturer/VideoCapturer.h): abstract interfaces defined for video capturer sensor/encoder.
- [AudioCapturer.h](include/com/amazonaws/kinesis/video/capturer/AudioCapturer.h): abstract interfaces defined for audio capturer sensor/encoder.
- [AudioPlayer.h](include/com/amazonaws/kinesis/video/player/AudioPlayer.h): abstract interfaces defined for audio playback sensor/encoder.

The implementations of those interfaces should be put into *source/${BOARD_NAME}* and follow the name rules:
- `${BOARD_NAME}VideoCapturer.c`
- `${BOARD_NAME}AudioCapturer.c`
- `${BOARD_NAME}AudioPlayer.c`

After implementation, you also need to create a platform specific CMake file in [CMake](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/tree/master/CMake) named as `${BOARD_NAME}.cmake`:
```cmake
if(BOARD STREQUAL "BOARD_NAME")
    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})
    # Add board related SDK sources here
    set(BOARD_SRCS
    )
    # Add board related SDK header here
    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/include/
        ${BOARD_SDK_DIR}/samples/libimp-samples/
    )
    # Add board related SDK lib here
    set(BOARD_LIBS_SHARED
    )
    set(BOARD_LIBS_STATIC
    )
endif()

```

## Documentations

[Documentations](doc/guide.md)
## Contributing

[Contributing Guide](CONTRIBUTING.md)

## Code Of Conduct

[Code Of Conduct](CODE_OF_CONDUCT.md)

## License

[Apache 2.0](LICENSE)
