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

#ifndef SAMPLE_OPTIONS_H
#define SAMPLE_OPTIONS_H

/**
 * @brief Get AWS access key
 *
 * It searches AWS access key with following order:
 *      1. environment variable
 *      2. sample_config.h
 *
 * @return AWS access key
 */
const char *OptCfg_getAwsAccessKey();

/**
 * @brief Get AWS secret access key
 *
 * It searches AWS secret access key with following order:
 *      1. environment variable
 *      2. sample_config.h
 *
 * @return AWS secret access key
 */
const char *OptCfg_getAwsSecretAccessKey();

/**
 * @brief Get AWS region
 *
 * It searches AWS region with following order:
 *      1. environment variable
 *      2. sample_config.h
 *
 * @return AWS region
 */
const char *OptCfg_getRegion();

/**
 * @brief Get AWS KVS service name
 *
 * @return KVS service name
 */
const char *OptCfg_getServiceKinesisVideo();

/**
 * @brief Get KVS host name
 *
 * It searches KVS host name with following order:
 *      1. environment variable
 *      2. sample_config.h
 *
 * @return KVS host name
 */
const char *OptCfg_getHostKinesisVideo();

#endif /* SAMPLE_OPTIONS_H */
