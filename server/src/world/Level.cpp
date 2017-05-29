//
// Created by ldavid on 5/23/17.
//

#include "Level.hpp"

using namespace world;

Level::Level(data::Level::Ptr data, const std::shared_ptr<World> &world)
    : mData(data), mWorld(world)
{

}

void Level::doTick(size_t tickId)
{
    for (auto entity : mEntities) {
        entity->doTick(tickId);
    }
}

data::Level::Ptr &Level::data()
{
    return mData;
}

std::shared_ptr<Entity> Level::entityAt(int x, int y)
{
    auto it = std::find_if(mEntities.begin(), mEntities.end(), [&](auto& entity) -> bool {
        return entity->position().x == x && entity->position().y == y;
    });

    if (it != mEntities.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

void Level::addEntity(std::shared_ptr<Entity> entity)
{
    entity->attach(shared_from_this());
    mEntities.insert(entity);
}

void Level::removeEntity(std::shared_ptr<Entity> entity)
{
    mEntities.erase(entity);
}

std::set<std::shared_ptr<Entity>> &Level::entities()
{
    return mEntities;
}
