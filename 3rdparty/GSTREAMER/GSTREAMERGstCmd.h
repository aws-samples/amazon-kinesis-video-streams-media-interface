/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#pragma once

// video:
// gst-launch-1.0 -e rtspsrc location=rtsp://192.168.0.60:554/stream0 latency=500 ! rtph264depay ! queue ! h264parse config-interval=1 ! capsfilter caps=video/x-h264,stream-format=(string)byte-stream,alignment=(string)au ! mpegtsmux !  tcpserversink host=127.0.0.1 port=8501 &
// gst-launch-1.0 -e tcpclientsrc host=127.0.0.1 port=8501 ! tsdemux ! h264parse ! capsfilter caps="video/x-h264,stream-format=(string)byte-stream,alignment=(string)au" ! appsink name=sink
#define GST_LAUNCH_VIDEO_PIPELINE_CMD                                                                  \
    "tcpclientsrc host=%s port=%s ! tsdemux ! h264parse ! capsfilter caps=video/x-h264,stream-format=(string)byte-stream,alignment=(string)au ! appsink name=%s"

// audio:
// gst-launch-1.0 -e rtspsrc location=rtsp://192.168.0.60:554/stream0 latency=500 ! rtppcmadepay ! queue ! tcpserversink host=127.0.0.1 port=8503 &
// "tcpclientsrc host=127.0.0.1 port=8503 ! appsink name=sink"
#define GST_LAUNCH_AUDIO_PIPELINE_CMD                                                                  \
    "tcpclientsrc host=%s port=%s ! appsink name=%s"

// audio player:
// "appsrc name=alawsrc ! capsfilter caps=audio/x-alaw,rate=8000,channels=1 ! alawdec ! autoaudiosink"
#define GST_LAUNCH_AUDIO_PLAYER_PIPELINE_CMD                                                           \
    "appsrc name=%s ! capsfilter caps=audio/x-alaw,rate=8000,channels=1 ! alawdec ! autoaudiosink"
