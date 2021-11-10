# Amazon Kinesis Video Streams Media Interface

## Introduction

**Amazon Kinesis Video Streams Media Interface** provides abstract interfaces for board specific media APIs. This repository also contains SoCs sensors/encoder implementations for Amazon Kinesis Video Streams Producer and WebRTC with out-of-box samples.

## Supported SoCs

- T31

## Getting Started with out-of-box KVS WebRTC sample

1. Generate AWS credentials by using this [script](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/master/scripts/generate-iot-credential.sh)
2. Copy SoC SDK dependencies into *3rdparty/$BOARD/*. See [3rdparty/README.md](3rdparty/README.md).
3. Build the code: `cd amazon-kinesis-video-streams-audio-video-interface; mkdir build; cd build; cmake ..; make`.
4. Upload built artifacts at *amazon-kinesis-video-streams-audio-video-interface/build* to your board.
5. Setup AWS credentials in enjoinment variables. The sample is using AWS IoT certificates by default:

```bash
export AWS_KVS_LOG_LEVEL=2
export AWS_DEFAULT_REGION=us-east-1
export AWS_KVS_CACERT_PATH=cacert.pem
export AWS_IOT_CORE_THING_NAME=your_camera_name
export AWS_IOT_CORE_CREDENTIAL_ENDPOINT=xxxxxxxxxxxxxx.credentials.iot.us-east-1.amazonaws.com
export AWS_IOT_CORE_CERT=your_camera_certificate.pem
export AWS_IOT_CORE_PRIVATE_KEY=your_camera_private.key
export AWS_IOT_CORE_ROLE_ALIAS=your_camera_role_alias
```

6. Execute sample: `./kvswebrtcmaster-static`

## Platform Implementation Guide

To adapt other platforms SDKs with amazon-kinesis-video-streams-capturer, you need to implement interfaces in *include/com/amazonaws/kinesis/video/capturer/VideoCapturer.h*, *include/com/amazonaws/kinesis/video/capturer/AudioCapturer.h* and *include/com/amazonaws/kinesis/video/player/AudioPlayer.h*:

- [VideoCapturer.h](include/com/amazonaws/kinesis/video/capturer/VideoCapturer.h): abstract interfaces defined for video capturer sensor/encoder.
- [AudioCapturer.h](include/com/amazonaws/kinesis/video/capturer/AudioCapturer.h): abstract interfaces defined for audio capturer sensor/encoder.
- [AudioPlayer.h](include/com/amazonaws/kinesis/video/player/AudioPlayer.h): abstract interfaces defined for audio playback sensor/encoder.

## Contributing

[Contributing Guide](CONTRIBUTING.md)

## Code Of Conduct

[Code Of Conduct](CODE_OF_CONDUCT.md)

## License

[Apache 2.0](LICENSE)
