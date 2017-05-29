//
// Created by ldavid on 5/22/17.
//

#include <aws/gamelift/GameLiftClient.h>
#include <data/Loader.hpp>

#ifndef PREZ_GAME_STATIC_HPP
#define PREZ_GAME_STATIC_HPP

extern Aws::GameLift::GameLiftClient* glClient;

extern data::Loader* dataLoader;

#endif //PREZ_GAME_STATIC_HPP
