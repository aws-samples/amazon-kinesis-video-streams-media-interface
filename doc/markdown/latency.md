# KVS/WebRTC SDK latency optimization checklist

## Device(Camera):
* DNS resolving time consuming
* Modify the camera application to use the persistent connection
    * Dedicated signaling channel per device
    * Cache the signaling channel endpoint 
        * To modify the policy of caching configuration: In WebRTC C SDK, “samples/Common.c”, modify “pSampleConfiguration→channelInfo.cachingPolicy = SIGNALING_API_CALL_CACHE_TYPE_FILE;” ENUM is defined in “src/include/com/amazonaws/kinesis/video/webrtcclient/Include.h”
    * Cache the TURN channel configure(5min), try to optimize DNS first
    * skip TURN DNS, just use ip address in the TURN URI
* Apply ice filtering to exclude the IPv6 addresses
    * Disable IPv6 in device (generally). Disable IPv6 on master(camera) OS(etc. Linux)
    * Check device SDK will try IPv6 candidate from viewer or not
* Add configuration for TURN On/Off
    * To modify ICE mode (TURN/STUN), please modify the code in WebRTC C SDK “sample/Common.c”: “configuration.iceTransportPolicy = ICE_TRANSPORT_POLICY_ALL;”,  ENUM is defined in “src/include/com/amazonaws/kinesis/video/webrtcclient/Include.h”
* Enable trickleIce using createSampleConfiguration and modify third parameter to true
    * When connect to Echo, please turn it off
* Correlate the KVS Side metrics for WebRTC - send SDP offer/Answer link
    * API response latency/failure inside AWS service
* KVS WebRTC Test Page
    * Google Chrome so that we can view the chrome://webrtc-internals
* Cloudwatch metrics for KVS WebRTC

## App (user):

* In video player of mobile App, preload the WebRTC process before user click play button;
* Cache the signaling channel endpoint, and periodically refresh the cached data; For input parameter, use the largest valid time value;
    * Timeout, refresh cycle is 1 hour.
* Enable trickle ICE (both App & Device);
    * search code, look for “trickleICE”, and modify 


### How to optimize DNS resolving time consuming
1. In `src/source/Ice/Network.c`, find `STATUS getIpWithHostName(PCHAR hostname, PKvsIpAddress destIp)` and insert timestamp measurment.  
![](./WebRTC%20DNS%20latency.png)
2. If getIpWithHostName() takes long time (>1s), latency is caused by DNS resolving service
3. Please
    - modify devices hostname file(Linux) to change DNS resolver.
    - contact IC module (e.g. Ingenic/Ankya...) supplier.
    - Parse the TURN url and return IP address.