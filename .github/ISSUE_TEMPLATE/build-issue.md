---
name: Build issue
about: Describe the build issue you met
title: "[BUILD ISSUE]"
labels: ''
assignees: ''

---

**Describe the build issue**
A clear and concise description of what the build issue is.

**Target platform**
The target platform you are using.

**Commit id**
The commit id you are using.
47c3e59d9d14df0be7e8532279646c7a63c5b6de

**Build commands**
The build command you are using.
```
export CC=${YOUR_C_TOOLCHAIN}
export CXX=${YOUR_CXX_TOOLCHAIN}
cd amazon-kinesis-video-streams-media-interface
mkdir build; cd build; cmake .. -DBOARD=${YOUR_BOARD_NAME}
make
```

**Additional context**
Add any other context about the problem here.
