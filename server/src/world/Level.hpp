//
// Created by ldavid on 5/23/17.
//

#include <data/Level.hpp>
#include "Entity.hpp"

#ifndef PREZ_GAME_WORLD_LEVEL_HPP
#define PREZ_GAME_WORLD_LEVEL_HPP

namespace world
{
    class World;

    class Level : public std::enable_shared_from_this<Level>
    {
    public:
        Level(data::Level::Ptr data, const std::shared_ptr<World>& world);

        void doTick(size_t tickId);

        data::Level::Ptr &data();

        std::set<std::shared_ptr<Entity>> &entities();
        void addEntity(std::shared_ptr<Entity> entity);
        void removeEntity(std::shared_ptr<Entity> entity);

        std::shared_ptr<Entity> entityAt(int x, int y);

    private:
        std::set<std::shared_ptr<Entity>> mEntities;
        std::shared_ptr<World> mWorld;
        data::Level::Ptr mData;
    };

}


#endif //PREZ_GAME_LEVEL_HPP
