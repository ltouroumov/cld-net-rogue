//
// Created by ldavid on 6/1/17.
//

#include <string>

#ifndef PREZ_GAME_CLIENTCONFIG_HPP
#define PREZ_GAME_CLIENTCONFIG_HPP


class ClientConfig
{
public:
    ClientConfig(const std::string& configFile);

    const std::string &getAwsAccessKeyId() const;

    const std::string &getAwsSecretKey() const;

    const std::string &getAliasId() const;

    const std::string &getAwsRegion() const;

private:
    void loadConfig(const std::string& configFile);
    std::string mAwsAccessKeyId;
    std::string mAwsSecretKey;
    std::string mAliasId;
    std::string mAwsRegion;
};


#endif //PREZ_GAME_CLIENTCONFIG_HPP
