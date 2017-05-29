//
// Created by ldavid on 5/23/17.
//

#include <memory>
#include <unordered_map>

#include "Entity.hpp"

#ifndef PREZ_GAME_WORLDHANDLER_HPP
#define PREZ_GAME_WORLDHANDLER_HPP

class ClientHandler;

namespace world
{
    class World;

    class Handler
    {
    public:
        Handler(World &world);

        void update(size_t tickId);

        std::shared_ptr<Entity> enter(const std::shared_ptr<ClientHandler> &clientHandler);

        void leave(const std::shared_ptr<ClientHandler> &clientHandler);

    private:
        std::shared_ptr<Entity> addPlayerEntity();

        world::World &mWorld;

        std::unordered_map<std::shared_ptr<ClientHandler>, std::shared_ptr<Entity>> mClients;
    };
}


#endif //PREZ_GAME_WORLDHANDLER_HPP
