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

#define V4L2_HANDLE_NULL_CHECK(x)                                                                                                                    \
    if (!(x)) {                                                                                                                                      \
        return -EINVAL;                                                                                                                              \
    }
#define V4L2_HANDLE_STATUS_CHECK(v4l2Handle, expectedStatus)                                                                                         \
    if ((v4l2Handle)->status != (expectedStatus)) {                                                                                                  \
        return -EAGAIN;                                                                                                                              \
    }

#define LOG(msg, ...) printf(msg "\n", ##__VA_ARGS__)
