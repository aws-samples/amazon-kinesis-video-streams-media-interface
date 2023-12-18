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

// camera0:
// gst-launch-1.0 -e qtiqmmfsrc name=camsrc ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=25/1 ! queue ! omxh264enc control-rate=max-bitrate target-bitrate=6000000 interval-intraframes=24 periodicity-idr=1 ! queue ! h264parse config-interval=1 ! mpegtsmux ! queue ! tcpserversink host=127.0.0.1 port=8501 &
// "tcpclientsrc host=127.0.0.1 port=8501 ! tsdemux ! h264parse ! capsfilter caps=video/x-h264,stream-format=(string)byte-stream,alignment=(string)au ! appsink name=sink"
// camera1:
// gst-launch-1.0 -e qtiqmmfsrc name=camsrc camera=1 ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=25/1 ! queue ! omxh264enc control-rate=max-bitrate target-bitrate=6000000 interval-intraframes=24 periodicity-idr=1 ! queue ! h264parse config-interval=1 ! mpegtsmux ! queue ! tcpserversink host=127.0.0.1 port=8502 &
// "tcpclientsrc host=127.0.0.1 port=8502 ! tsdemux ! h264parse ! capsfilter caps=video/x-h264,stream-format=(string)byte-stream,alignment=(string)au ! appsink name=sink"
#define GST_LAUNCH_VIDEO_PIPELINE_CMD                                                                  \
    "tcpclientsrc host=%s port=%s ! tsdemux ! h264parse ! capsfilter caps=video/x-h264,stream-format=(string)byte-stream,alignment=(string)au ! appsink name=%s"

// audio:
// gst-launch-1.0 pulsesrc volume=100 ! capsfilter caps=audio/x-raw,format=S16LE,rate=8000,channels=2 ! audioconvert ! capsfilter caps=audio/x-raw,format=S16LE,rate=8000,channels=1 ! alawenc ! queue ! tcpserversink host=127.0.0.1 port=8503 &
// "tcpclientsrc host=127.0.0.1 port=8503 ! appsink name=sink"
#define GST_LAUNCH_AUDIO_PIPELINE_CMD                                                                  \
    "tcpclientsrc host=%s port=%s ! appsink name=%s"

// audio player:
// "appsrc name=alawsrc ! capsfilter caps=audio/x-alaw,rate=8000,channels=1 ! alawdec ! queue ! pulsesink volume=1"
#define GST_LAUNCH_AUDIO_PLAYER_PIPELINE_CMD                                                           \
    "appsrc name=%s ! capsfilter caps=audio/x-alaw,rate=8000,channels=1 ! alawdec ! queue ! pulsesink volume=1"
