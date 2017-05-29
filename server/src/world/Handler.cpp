//
// Created by ldavid on 5/23/17.
//

#include <random>
#include "Handler.hpp"
#include "World.hpp"
#include "../ClientHandler.hpp"

using namespace world;

Handler::Handler(world::World &world)
    : mWorld(world)
{

}

void Handler::update(size_t tickId)
{
    for (auto& pair : mClients) {
        std::shared_ptr<ClientHandler> client;
        std::shared_ptr<Entity> player;
        std::tie(client, player) = pair;

        net::Tick tickMsg;
        tickMsg.set_tick_id(tickId);
        tickMsg.set_map_name(player->level()->data()->name());

        for (auto& entity : player->level()->entities()) {
            net::Tick_Entity* netEntity = tickMsg.add_entities();
            netEntity->set_id(entity->id());
            netEntity->set_x(entity->position().x);
            netEntity->set_y(entity->position().y);
            netEntity->set_icon(entity->display().icon);
        }

        client->Send(tickMsg);
    }
}

std::shared_ptr<Entity> Handler::enter(const std::shared_ptr<ClientHandler> &clientHandler)
{
    std::shared_ptr<Entity> playerEntity = addPlayerEntity();
    mClients.insert(std::make_pair(clientHandler, playerEntity));
    return playerEntity;
}

void Handler::leave(const std::shared_ptr<ClientHandler> &clientHandler)
{
    auto it = mClients.find(clientHandler);
    it->second->kill();
    mClients.erase(it);

}

std::shared_ptr<Entity> Handler::addPlayerEntity()
{
    auto logger = spdlog::get("main");

    std::shared_ptr<Level>& topLevel = mWorld.levels().at(0);
    data::Level::Ptr& levelData = topLevel->data();

    std::shared_ptr<Entity> playerEntity = mWorld.makeEntity(new PlayerController());
    Entity::Controller* controller = playerEntity->controller();
    playerEntity->display().icon = '@';

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> xRand(0, levelData->size().width - 1);
    std::uniform_int_distribution<> yRand(0, levelData->size().height - 1);
    int xPos = 0;
    int yPos = 0;
    do {
        xPos = xRand(gen);
        yPos = yRand(gen);
    } while (levelData->at(xPos, yPos).solid || topLevel->entityAt(xPos, yPos));

    playerEntity->position() = Entity::Position(xPos, yPos);

    topLevel->addEntity(playerEntity);

    logger->debug("Placed player at {}:{}", xPos, yPos);

    return playerEntity;
}
