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
