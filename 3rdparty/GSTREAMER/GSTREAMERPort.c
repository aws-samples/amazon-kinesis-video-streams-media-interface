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
#include <stddef.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>

#include "GSTREAMERPort.h"

#define FILEEXTENSION GstCmd.h
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)
#define INCLUDE_FILE(name, tail) STRINGIFY(CONCATENATE(name, tail))
#include INCLUDE_FILE(BOARD, FILEEXTENSION)

#define GST_TCP_HOST           "127.0.0.1"
#define GST_VIDEO_TCP_PORT     "8501"
#define GST_AUDIO_TCP_PORT     "8503"
#define GST_SIGNAL_CALLBACK_NEW_SAMPLE   "new-sample"
#define GST_SIGNAL_CALLBACK_MSG_ERROR    "message::error"
#define GST_SIGNAL_CALLBACK_MSG_EOS      "message::eos"

typedef enum {
    VIDEO_CAPTURE_APP_SINK,
    AUDIO_CAPTURE_APP_SINK,
    AUDIO_PLAYER_APP_SRC
} GSTAppModuleType;

static GstElement *g_pipeline[3]; // 0: videoCapture; 1: audioCapture; 2: audioPlayer
static bool g_gstInit = false;

/**
 * @brief the callback is invoked when the error happens on the bus.
 *
 * @param[in] bus the bus of the callback.
 * @param[in] msg the msg of the callback.
 * @param[in] udata the user data.
 */
static void onMsgErrorFromBus(GstBus* bus, GstMessage* msg, gpointer* udata)
{
    GError* err;
    gchar* debug_info;

    if ((bus == NULL) || (msg == NULL)) {
        return;
    }

    gst_message_parse_error(msg, &err, &debug_info);
    if (err != NULL) {
        printf("error code: %d\n", err->code);
        printf("error received from element %s: %s\n", gst_object_get_name(msg->src), err->message);
    }
    if (debug_info != NULL) {
        printf("debugging information: %s\n", debug_info);
    }

    g_error_free(err);
    g_free(debug_info);

    return;
}

/**
 * @brief the callback is invoked when the end of stream happens on the bus.
 *
 * @param[in] bus the bus of the callback.
 * @param[in] msg the msg of the callback.
 * @param[in] udata the user data.
 */
static void onMsgEosFromBus(GstBus* bus, GstMessage* msg, gpointer* udata)
{
    if ((bus == NULL) || (msg == NULL)) {
        return;
    }

    printf("msg EOS\n");

    return;
}

static int initGstAppSinkSrc(GSTAppSinkSrc *handle, const GSTAppModuleType type)
{
    GstElement *pipeline = NULL;
    GstBus *bus = NULL;
    GstElement *src = NULL, *sink = NULL;
    char *port = NULL;
    char cmd[512] = { 0 };

    if (!g_gstInit) {
        gst_init(NULL, NULL);
        g_gstInit = true;
    }

    switch (type) {
        case VIDEO_CAPTURE_APP_SINK:
            port = getenv("VIDEO_PORT");
            port = (NULL != port) ? port : GST_VIDEO_TCP_PORT;
            snprintf(cmd, sizeof(cmd), GST_LAUNCH_VIDEO_PIPELINE_CMD, GST_TCP_HOST, port, GST_VIDEO_SINK_NAME);
            g_pipeline[VIDEO_CAPTURE_PIPELINE] = gst_parse_launch(cmd, NULL);
            pipeline = g_pipeline[VIDEO_CAPTURE_PIPELINE];
            sink = gst_bin_get_by_name(GST_BIN(pipeline), GST_VIDEO_SINK_NAME);
            break;
        case AUDIO_CAPTURE_APP_SINK:
            port = getenv("AUDIO_PORT");
            port = (NULL != port) ? port : GST_AUDIO_TCP_PORT;
            snprintf(cmd, sizeof(cmd), GST_LAUNCH_AUDIO_PIPELINE_CMD, GST_TCP_HOST, port, GST_AUDIO_SINK_NAME);
            g_pipeline[AUDIO_CAPTURE_PIPELINE] = gst_parse_launch(cmd, NULL);
            pipeline = g_pipeline[AUDIO_CAPTURE_PIPELINE];
            sink = gst_bin_get_by_name(GST_BIN(pipeline), GST_AUDIO_SINK_NAME);
            break;
        case AUDIO_PLAYER_APP_SRC:
            snprintf(cmd, sizeof(cmd), GST_LAUNCH_AUDIO_PLAYER_PIPELINE_CMD, GST_AUDIO_SRC_NAME);
            g_pipeline[AUDIO_PLAYER_PIPELINE] = gst_parse_launch(cmd, NULL);
            pipeline = g_pipeline[AUDIO_PLAYER_PIPELINE];
            src = gst_bin_get_by_name(GST_BIN(pipeline), GST_AUDIO_SRC_NAME);
            break;
        default:
            printf("error type: %d!\n", type);
            return -1;
    }
    if (NULL == pipeline || (NULL == src && NULL == sink)) {
        printf("pipeline or src or sink is NULL!\n");
        return -1;
    }

    // Instruct the bus to emit signals for each received message, and connect to the interesting signals
    if ((bus = gst_element_get_bus(pipeline)) != NULL) {
        gst_bus_add_signal_watch(bus);
        g_signal_connect(bus, GST_SIGNAL_CALLBACK_MSG_ERROR, G_CALLBACK(onMsgErrorFromBus), NULL);
        g_signal_connect(bus, GST_SIGNAL_CALLBACK_MSG_EOS, G_CALLBACK(onMsgEosFromBus), NULL);
    }

    if (NULL != src) {
        g_object_set(src, "is-live", TRUE, NULL);
    }

    return 0;
}

static int startGstAppSinkSrc(const GSTAppModuleType type)
{
    GstElement *pipeline = NULL;
    switch (type) {
        case VIDEO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[VIDEO_CAPTURE_PIPELINE];
            break;
        case AUDIO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[AUDIO_CAPTURE_PIPELINE];
            break;
        case AUDIO_PLAYER_APP_SRC:
            pipeline = g_pipeline[AUDIO_PLAYER_PIPELINE];
            break;
        default:
            printf("error type: %d!\n", type);
            return -1;
    }

    if (NULL == pipeline) {
        printf("pipeline is NULL!\n");
        return -1;
    }

    if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(pipeline, GST_STATE_PLAYING)) {
        printf("gst_element_set_state failed!\n");
        return -1;
    }

    return 0;
}

static int stopGstAppSinkSrc(const GSTAppModuleType type)
{
    GstElement *pipeline = NULL;
    switch (type) {
        case VIDEO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[VIDEO_CAPTURE_PIPELINE];
            break;
        case AUDIO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[AUDIO_CAPTURE_PIPELINE];
            break;
        case AUDIO_PLAYER_APP_SRC:
            pipeline = g_pipeline[AUDIO_PLAYER_PIPELINE];
            break;
        default:
            printf("error type: %d!\n", type);
            return -1;
    }

    if (NULL == pipeline) {
        printf("pipeline is NULL!\n");
        return -1;
    }

    if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(pipeline, GST_STATE_NULL)) {
        printf("gst_element_set_state failed!\n");
        return -1;
    }

    return 0;
}

static int deinitGstAppSinkSrc(const GSTAppModuleType type)
{
    GstElement *pipeline = NULL;
    switch (type) {
        case VIDEO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[VIDEO_CAPTURE_PIPELINE];
            break;
        case AUDIO_CAPTURE_APP_SINK:
            pipeline = g_pipeline[AUDIO_CAPTURE_PIPELINE];
            break;
        case AUDIO_PLAYER_APP_SRC:
            pipeline = g_pipeline[AUDIO_PLAYER_PIPELINE];
            break;
        default:
            printf("error type: %d!\n", type);
            return -1;
    }

    if (NULL == pipeline) {
        printf("pipeline is NULL!\n");
        return -1;
    }

    gst_object_unref(pipeline);

    return 0;
}

int initGstVideoSrc(GSTAppSinkSrc *handle)
{
    return initGstAppSinkSrc(handle, VIDEO_CAPTURE_APP_SINK);
}

int startGstVideoSrc(void)
{
    return startGstAppSinkSrc(VIDEO_CAPTURE_APP_SINK);
}

int stopGstVideoSrc(void)
{
    return stopGstAppSinkSrc(VIDEO_CAPTURE_APP_SINK);
}

int deinitGstVideoSrc(void)
{
    return deinitGstAppSinkSrc(VIDEO_CAPTURE_APP_SINK);
}

int initGstAudioSrc(GSTAppSinkSrc *handle)
{
    return initGstAppSinkSrc(handle, AUDIO_CAPTURE_APP_SINK);
}

int startGstAudioSrc(void)
{
    return startGstAppSinkSrc(AUDIO_CAPTURE_APP_SINK);
}

int stopGstAudioSrc(void)
{
    return stopGstAppSinkSrc(AUDIO_CAPTURE_APP_SINK);
}

int deinitGstAudioSrc(void)
{
    return deinitGstAppSinkSrc(AUDIO_CAPTURE_APP_SINK);
}

int initGstAudioPlayer(GSTAppSinkSrc *handle)
{
    return initGstAppSinkSrc(handle, AUDIO_PLAYER_APP_SRC);
}

int startGstAudioPlayer(void)
{
    return startGstAppSinkSrc(AUDIO_PLAYER_APP_SRC);
}

int stopGstAudioPlayer(void)
{
    return stopGstAppSinkSrc(AUDIO_PLAYER_APP_SRC);
}

int deinitGstAudioPlayer(void)
{
    return deinitGstAppSinkSrc(AUDIO_PLAYER_APP_SRC);
}

int writeToGstAudioPlayer(uint8_t *pData, size_t size)
{
    GstFlowReturn ret;
    GstElement *pipeline = g_pipeline[AUDIO_PLAYER_PIPELINE];
    GstElement *src = gst_bin_get_by_name(GST_BIN(pipeline), GST_AUDIO_SRC_NAME);

    GstBuffer *buffer = gst_buffer_new_and_alloc(size);
    if (buffer) {
        gst_buffer_fill(buffer, 0, pData, size);
        g_signal_emit_by_name(src, "push-buffer", buffer, &ret);
        gst_buffer_unref(buffer);
    }

    if (ret != GST_FLOW_OK) {
        printf("Error pushing buffer to appsrc\n");
        return -1;
    }

    return 0;
}

__attribute__((weak)) uint64_t getEpochTimestampInUs(void)
{
    uint64_t timestamp = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    timestamp = (uint64_t) (tv.tv_sec) * 1000 * 1000 + (uint64_t) (tv.tv_usec);

    return timestamp;
}

GstElement * getPipeline(int type)
{
    return g_pipeline[type];
}
