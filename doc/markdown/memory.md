# WebRTC Memory Optimization Guide

## Third party libraries

### libssl + libcrypto

When use OpenSSL, libssl + libcrypto will use 2M ~ 4M RAM:


* Optimize suggestion: Use mbedTLS instead of OpenSSL. This replacement can reduce around 1M RAM usage with one viewer during our test.

### libwebsockets + libusrsctp

This two libraries will use 200KB ~ 500KB RAM.

# WebRTC C SDK

## How to optimize WebRTC C SDK memory(RAM/Flash)?
1. Using mbedTLS instead of OpenSSL. This replacement can reduce around 1M RAM usage with one viewer during our test.
2. Reducing Rolling buffer size. SDK itself will maintain a 3 seconds rolling buffer by default(DEFAULT_ROLLING_BUFFER_DURATION_IN_SECONDS * HIGHEST_EXPECTED_BIT_RATE / 8 / DEFAULT_MTU_SIZE). Reduce rolling buffer according to your user cases at Rtp.h, i.e.:
```
 #define DEFAULT_MTU_SIZE                           1200
 #define DEFAULT_ROLLING_BUFFER_DURATION_IN_SECONDS 1
 #define HIGHEST_EXPECTED_BIT_RATE                  (5 * 1024 * 1024)
 #define DEFAULT_SEQ_NUM_BUFFER_SIZE                1000
 #define DEFAULT_VALID_INDEX_BUFFER_SIZE            1000
 #define DEFAULT_PEER_FRAME_BUFFER_SIZE             (5 * 1024)
 #define SRTP_AUTH_TAG_OVERHEAD                     10

```
3. If your device don't need data channel, use CMake flag ENABLE_DATA_CHANNEL=OFF to disable it.
4. Build with gcc optimization flag and remove debug symbols.

### Rolling buffer information

SDK itself will maintain a 3 seconds rolling buffer(DEFAULT_ROLLING_BUFFER_DURATION_IN_SECONDS * HIGHEST_EXPECTED_BIT_RATE / 8 / DEFAULT_MTU_SIZE).
```
#define DEFAULT_MTU_SIZE                           1200
#define DEFAULT_ROLLING_BUFFER_DURATION_IN_SECONDS 3
#define HIGHEST_EXPECTED_BIT_RATE                  (10 * 1024 * 1024)
#define DEFAULT_SEQ_NUM_BUFFER_SIZE                1000
#define DEFAULT_VALID_INDEX_BUFFER_SIZE            1000
#define DEFAULT_PEER_FRAME_BUFFER_SIZE             (5 * 1024)
#define SRTP_AUTH_TAG_OVERHEAD                     10
```
* Optimize suggestion: Reduce rolling buffer according to your user cases. i.e.:
```
#define DEFAULT_MTU_SIZE                           1200
#define DEFAULT_ROLLING_BUFFER_DURATION_IN_SECONDS 1
#define HIGHEST_EXPECTED_BIT_RATE                  (5 * 1024 * 1024)
#define DEFAULT_SEQ_NUM_BUFFER_SIZE                1000
#define DEFAULT_VALID_INDEX_BUFFER_SIZE            1000
#define DEFAULT_PEER_FRAME_BUFFER_SIZE             (5 * 1024)
#define SRTP_AUTH_TAG_OVERHEAD                     10

```
## Datachannel

Datachannel can take hundreds of bytes RAM, depends on messages you trying to send/receive.

* Optimize suggestion: In Release 1.4.0, We introduced an option in CMAKE to disable datachannel. If your device don’t need datachannel, you can disable it via flag ENABLE_DATA_CHANNEL=OFF

# Result
## KVS/WebRTC on Ingenic T31:
* RAM(VMRSS)::
    * current minimal, 1MB rolling buffer: 6.5 MB (One viewer)
    * Add 1.5 MB per viewer
* Flash:
    * static build, minimal 1.1 MB
* CPU: about 20%


# Releases

1.4.0

v1.4.0 at 2020/11/11, We introduced various of new designs to reduce RAM usage, i.e. a hash table based JitterBuffer.

* Optimize suggestion: Update from 1.3.0 to 1.4.0 can reduce RAM usage around 300KB(Depends on numbers of your viewers)

