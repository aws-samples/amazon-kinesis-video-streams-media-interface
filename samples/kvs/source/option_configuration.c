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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option_configuration.h"
#include "sample_config.h"

#define AWS_ACCESS_KEY_ENV_VAR          "AWS_ACCESS_KEY_ID"
#define AWS_SECRET_KEY_ENV_VAR          "AWS_SECRET_ACCESS_KEY"
#define AWS_SESSION_TOKEN_ENV_VAR       "AWS_SESSION_TOKEN"
#define AWS_DEFAULT_REGION_ENV_VAR      "AWS_DEFAULT_REGION"

#define AWS_KINESIS_VIDEO_HOST_ENV_VAR  "AWS_KVS_HOST"

const char *OptCfg_getAwsAccessKey()
{
    char *pAwsAccessKey = NULL;

#ifdef SAMPLE_OPTIONS_FROM_ENV_VAR
    pAwsAccessKey = getenv(AWS_ACCESS_KEY_ENV_VAR);
#endif /* SAMPLE_OPTIONS_FROM_ENV_VAR */

    if (pAwsAccessKey == NULL)
    {
        pAwsAccessKey = AWS_ACCESS_KEY;
    }

    return pAwsAccessKey;
}

const char *OptCfg_getAwsSecretAccessKey()
{
    char *pAwsSecretAccessKey = NULL;

#ifdef SAMPLE_OPTIONS_FROM_ENV_VAR
    pAwsSecretAccessKey = getenv(AWS_SECRET_KEY_ENV_VAR);
#endif /* SAMPLE_OPTIONS_FROM_ENV_VAR */

    if (pAwsSecretAccessKey == NULL)
    {
        pAwsSecretAccessKey = AWS_SECRET_KEY;
    }

    return pAwsSecretAccessKey;
}

const char *OptCfg_getAwsSessionToken()
{
    char *pAwsSessionToken = NULL;

#ifdef SAMPLE_OPTIONS_FROM_ENV_VAR
    pAwsSessionToken = getenv(AWS_SESSION_TOKEN_ENV_VAR);
#endif /* SAMPLE_OPTIONS_FROM_ENV_VAR */

    if (pAwsSessionToken == NULL)
    {
        pAwsSessionToken = strcmp(AWS_SESSION_TOKEN, "") ? AWS_SESSION_TOKEN : NULL;
    }

    return pAwsSessionToken;
}

const char *OptCfg_getRegion()
{
    char *pRegion = NULL;

#ifdef SAMPLE_OPTIONS_FROM_ENV_VAR
    pRegion = getenv(AWS_DEFAULT_REGION_ENV_VAR);
#endif /* SAMPLE_OPTIONS_FROM_ENV_VAR */

    if (pRegion == NULL)
    {
        pRegion = AWS_KVS_REGION;
    }

    return pRegion;
}

const char *OptCfg_getServiceKinesisVideo()
{
    return "kinesisvideo";
}

const char *OptCfg_getHostKinesisVideo()
{
    char *pKvsHost = NULL;
    const char *pRegion = NULL;
    const char *pService = NULL;
    size_t uLen = 0;

#ifdef SAMPLE_OPTIONS_FROM_ENV_VAR
    pKvsHost = getenv(AWS_KINESIS_VIDEO_HOST_ENV_VAR);
#endif /* SAMPLE_OPTIONS_FROM_ENV_VAR */

    if (pKvsHost == NULL)
    {
        pKvsHost = AWS_KVS_HOST;
    }

    return pKvsHost;
}
