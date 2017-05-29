//
// Created by ldavid on 5/23/17.
//

#include <spdlog/spdlog.h>
#include "Entity.hpp"
#include "Level.hpp"

using namespace world;


Entity::Entity(size_t id, Entity::Controller* controller)
    : mId(id), mPosition(0, 0), mController(controller)
{
    auto logger = spdlog::get("main");
    logger->debug("Controller {}", typeid(mController).name());
}

Entity::~Entity()
{
    delete mController;
}

void Entity::doTick(size_t tickId)
{
    mController->think(*this, tickId);
}

void Entity::attach(std::shared_ptr<Level> level)
{
    auto logger = spdlog::get("main");
    logger->debug("Entity Attached");
    mLevel = level;
}

std::shared_ptr<Level> &Entity::level()
{
    return mLevel;
}

void Entity::kill()
{
    mLevel->removeEntity(shared_from_this());
    mLevel.reset();
}

const size_t &Entity::id()
{
    return mId;
}

Entity::Position &Entity::position()
{
    return mPosition;
}

Entity::Display &Entity::display()
{
    return mDisplay;
}

Entity::Controller *Entity::controller()
{
    return mController;
}

void Entity::Controller::think(Entity &entity, size_t tickId)
{
}

void PlayerController::think(Entity &entity, size_t tickId)
{
    auto logger = spdlog::get("main");
    if (mNextMove) {
        Entity::Position nextPos = entity.position();
        switch (mNextMove) {
            case 1:
                nextPos.y--;
                break;
            case 2:
                nextPos.x++;
                break;
            case 3:
                nextPos.y++;
                break;
            case 4:
                nextPos.x--;
                break;
            default:
                break;
        }

        if (!entity.level()->data()->at(nextPos.x, nextPos.y).solid && !entity.level()->entityAt(nextPos.x, nextPos.y)) {
            entity.position() = nextPos;
        }

        mNextMove = 0;
    }
}

void PlayerController::move(uint8_t nextMove)
{
    mNextMove = nextMove;
}
