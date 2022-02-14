/*
 * Copyright 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include <stdio.h>

#define FILE_HANDLE_NULL_CHECK(x)                                                                                                                    \
    if (!(x)) {                                                                                                                                      \
        return -EINVAL;                                                                                                                              \
    }
#define FILE_HANDLE_STATUS_CHECK(fileHandle, expectedStatus)                                                                                         \
    if ((fileHandle)->status != (expectedStatus)) {                                                                                                  \
        return -EAGAIN;                                                                                                                              \
    }

#define GET_FILE_SIZE(f, size)                                                                                                                       \
    do {                                                                                                                                             \
        fseek((f), 0, SEEK_END);                                                                                                                     \
        (size) = ftell((f));                                                                                                                         \
        rewind((f));                                                                                                                                 \
    } while (0)

#define CLOSE_FILE(f)                                                                                                                                \
    do {                                                                                                                                             \
        fclose((f));                                                                                                                                 \
        (f) = NULL;                                                                                                                                  \
    } while (0)

#define LOG(msg, ...) printf(msg "\n", ##__VA_ARGS__)

#define FRAME_FILE_PATH_MAX_LENGTH (512)
