# 指南
[EN](./guide.md)

1. 图像传感器及音视频编码 [概念](./pdf/Basic%20Video%20for%20KVS.pdf)
    1. 传感器
    2. RGB编码
    3. 像素
    4. 人眼特性
    5. 分辨率
    6. YUV编码
    7. 码率
    8. 音频基础概念
2. 流媒体基础 [概念](./pdf/Streaming%20Media%20Concepts%20for%20Beginners.pdf)
    1. Frame rate, Resolution, Bandwidth
    2. Bits, Bytes, KVS pricing model
    3. Media Containerization
    4. Video Fragmentation
    5. Video Formats & Encoding Codec (H.264/265, I/P/B-Frames, )
    6. Transcoding
    7. HLS, DASH
    8. KVS - Multi-track Media
3. KVS Stream 和 Producer SDK的 [介绍](./pdf/Intro%20to%20Kinesis%20Video%20Streams.pdf)
    1. KVS Stream相关概念和功能: Ingest, Store, Consume
    2. Producer SDK, Archived Media APIs (HLS/DASH Url, GetMedia, GetClip, GetImages...)
    3. KVS 最佳实践介绍
4. KVS Stream 新功能 [介绍](./pdf/Latest%20Features.pdf)
    1. Image delivery S3,
    2. Rekognition Video Events SVE,
    3. Edge Agent
5. 动手实验: 
    1. [快速落地智能云存储摄像机解决方案](https://aws.amazon.com/cn/blogs/china/foundation-of-fast-landing-smart-camera-solution/)
    2. [基于Amazon Kinesis Video Streams实现 IP Camera云存项目](https://aws.amazon.com/cn/blogs/china/realize-ip-camera-cloud-storage-project-based-on-amazon-kinesis-video-streams/)

6. (optional) 动手实验 : KVS Streams 图片识别功能
    1. [基于KVS Images构建视频流检测方案的集成与分析](https://aws.amazon.com/cn/blogs/china/integration-and-analysis-of-video-stream-detection-scheme-based-on-amazon-kinesis-video-stream-images/)

7. KVS WebRTC 的 [介绍](./pdf/Intro%20to%20KVS%20WebRTC.pdf)
    1. 基本概念: Signaling channels, STUN, TURN, ICE, SDP, RTP, SCTP
    2. KVS WebRTC 相关API
    3. WebRTC SDK的使用
    4. Service limits
8. Media Interface device SDK介绍
    1. KVS Media Interface [介绍](../README.md)
    2. [Device内存优化](./markdown/memory.md)
    3. [Device延迟优化](./markdown/latencylatency.md)
    4. [迁移指南](./markdown/migration_guide.md)
9. 动手实验: Media-Interface SDK
    1. [快速落地智能摄像机解决方案 云存储+WebRTC](https://aws.amazon.com/cn/blogs/china/fast-landing-smart-camera-solution-cloud-storage-webrtc/)
10. KVS设备端集成的调试工具
    1. [producer SDK test page](https://github.com/aws-samples/amazon-kinesis-video-streams-media-viewer)
    2. [WebRTC test page](https://github.com/codingspirit/kvs-webrtc-test-page)
11. [FAQ](https://w.amazon.com/bin/view/GCR_INFRA_SSA/Playbooks/IoTCenter)

