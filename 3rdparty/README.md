# Board SDK Install Guide

This documents provides guidelines to install board SDK required by this library.

# FILE

FILE capturer is a dummy capturer that read sample frames from [resources/frames/]. It doesn't require user to manually install any 3rdparty SDK.

# V4L2

V4L2 request libv4l2 on your build device. You may install it via:

```
sudo apt install libv4l-dev
```

This library requires V4L2 cameras with H.264 encoder supports(i.e. Pi Camera). To check your camera capabilities, you may use `v4l2-ctl --list-formats` to check if it can be use. Take Pi camera as example:

```
v4l2-ctl --list-formats
ioctl: VIDIOC_ENUM_FMT
	Type: Video Capture

	[0]: 'YU12' (Planar YUV 4:2:0)
	[1]: 'YUYV' (YUYV 4:2:2)
	[2]: 'RGB3' (24-bit RGB 8-8-8)
	[3]: 'JPEG' (JFIF JPEG, compressed)
	[4]: 'H264' (H.264, compressed)
	[5]: 'MJPG' (Motion-JPEG, compressed)
	[6]: 'YVYU' (YVYU 4:2:2)
	[7]: 'VYUY' (VYUY 4:2:2)
	[8]: 'UYVY' (UYVY 4:2:2)
	[9]: 'NV12' (Y/CbCr 4:2:0)
	[10]: 'BGR3' (24-bit BGR 8-8-8)
	[11]: 'YV12' (Planar YVU 4:2:0)
	[12]: 'NV21' (Y/CrCb 4:2:0)
	[13]: 'RX24' (32-bit XBGR 8-8-8-8)
```

It doesn't require other manually installation because CMake will download and build dependencies automatically.

# T31

User need to put T31 board SDK under [3rdparty/T31/](T31/):

```
T31/
|-- include
|   |-- imp
|   `-- sysutils
|-- lib
|   |-- glibc
|   `-- uclibc
`-- samples
    |-- libimp-samples
    `-- libsysutils-samples
```

# FH8626V100

User need to put FH8626V100 board SDK under [3rdparty/FH8626V100/](FH8626V100/):

```
FH8626V100/
├── common
├── include
└── lib
```
