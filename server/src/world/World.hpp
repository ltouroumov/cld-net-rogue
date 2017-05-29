//
// Created by ldavid on 5/12/17.
//

#include <data/Loader.hpp>
#include <asio/strand.hpp>
#include <asio/deadline_timer.hpp>
#include "data/Dungeon.hpp"
#include "Level.hpp"
#include "Handler.hpp"

#ifndef PREZ_GAME_WORLD_HPP
#define PREZ_GAME_WORLD_HPP

class ClientHandler;

namespace world
{
    class Handler;

    class World : public std::enable_shared_from_this<World>
    {
    public:
        const int TICK_DURATION = 500;

        World(asio::io_service &io_service, data::Loader &loader);

        void start(const std::string &dungeon);

        std::vector<std::shared_ptr<Level>> &levels();

        Handler& handler();

        template<typename ...Args>
        std::shared_ptr<Entity> makeEntity(Args&& ... args)
        {
            return std::make_shared<Entity>(++mEntityCounter, std::forward<Args>(args)...);
        }

    private:
        void nextTick();

        void doTick();

        size_t mTickId;
        size_t mEntityCounter = 0;

        Handler mHandler;

        std::vector<std::shared_ptr<Level>> mLevels;

        data::Dungeon::Ptr mDungeon;
        data::Loader &mLoader;
        asio::io_service::strand mWorkQueue;
        asio::deadline_timer mTickTimer;
    };

}

#endif //PREZ_GAME_WORLD_HPP
