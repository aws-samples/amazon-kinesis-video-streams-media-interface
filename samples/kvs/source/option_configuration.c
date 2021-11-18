#include <stdio.h>
#include <stdlib.h>

#include "option_configuration.h"
#include "sample_config.h"

#define AWS_ACCESS_KEY_ENV_VAR          "AWS_ACCESS_KEY_ID"
#define AWS_SECRET_KEY_ENV_VAR          "AWS_SECRET_ACCESS_KEY"
#define AWS_DEFAULT_REGION_ENV_VAR      "AWS_DEFAULT_REGION"
#define AWS_SESSION_TOKEN_ENV_VAR       "AWS_SESSION_TOKEN"

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
