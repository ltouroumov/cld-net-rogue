//
// Created by ldavid on 6/1/17.
//

#include <yaml-cpp/yaml.h>
#include "ClientConfig.hpp"

ClientConfig::ClientConfig(const std::string &configFile)
{
    loadConfig(configFile);
}

void ClientConfig::loadConfig(const std::string &configFile)
{
    YAML::Node config = YAML::LoadFile("./config.yaml");

    mAwsAccessKeyId = config["aws_access_key_id"].as<std::string>();
    mAwsSecretKey = config["aws_secret_key"].as<std::string>();
    mAliasId = config["aws_alias_id"].as<std::string>();
    mAwsRegion = config["aws_region"].as<std::string>();
}

const std::string &ClientConfig::getAwsAccessKeyId() const
{
    return mAwsAccessKeyId;
}

const std::string &ClientConfig::getAwsSecretKey() const
{
    return mAwsSecretKey;
}

const std::string &ClientConfig::getAliasId() const
{
    return mAliasId;
}

const std::string &ClientConfig::getAwsRegion() const
{
    return mAwsRegion;
}
