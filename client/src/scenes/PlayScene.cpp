//
// Created by ldavid on 5/22/17.
//

#include <spdlog/fmt/bundled/format.h>
#include <spdlog/spdlog.h>
#include "PlayScene.hpp"
#include "../static.hpp"

int PlayScene::CONSOLE_LAYER = 5;

PlayScene::PlayScene(Director *director, Aws::GameLift::Model::PlayerSession &playerSession,
                     std::shared_ptr<AsioNetworkClient> &network)
    : Scene(director), mPlayerSession(playerSession), mNetwork(network)
{}

void PlayScene::tick(double delta)
{
    auto term = rltk::term(PlayScene::CONSOLE_LAYER);
    term->clear();
    rltk::ecs_tick(delta);
    std::string tickStr = fmt::format("tick: {:04x}", mTickId);
    term->print(0, 0, tickStr, rltk::colors::RED, rltk::colors::BLACK);
}

void PlayScene::enter()
{
    _tickHandler = mNetwork->dispatcher().wire<net::Tick>(std::bind(&PlayScene::onTick, this, std::placeholders::_1));
}

void PlayScene::leave()
{
    mNetwork->dispatcher().unwire(_tickHandler);
}

void PlayScene::attached()
{
    rltk::gui->add_layer(
        CONSOLE_LAYER,
        0, 0, 1024, 768,
        "16x16",
        [](rltk::layer_t *layer, int w, int h) {
            layer->w = w;
            layer->h = h;
        }
    );

    rltk::add_system<map_render_system>(mLevel);
    rltk::add_system<entities_render_system>();
    rltk::add_system<player_system>(*mNetwork);
    rltk::ecs_configure();
}

void PlayScene::detached()
{
    rltk::gui->delete_layer(PlayScene::CONSOLE_LAYER);
}

void PlayScene::onTick(net::Tick &tick)
{
    auto logger = spdlog::get("default");

    mTickId = tick.tick_id();
    if (!mLevel || mLevel->name() != tick.map_name()) {
        mLevel = dataLoader->getLevel(tick.map_name());
        logger->debug("Loaded level {}", mLevel->name());
    }

    for (const net::Tick_Entity &entity : tick.entities()) {

        rltk::entity_t* ecsEntity = rltk::entity(entity.id());
        if (ecsEntity) {
            position_t* pos = ecsEntity->component<position_t>();
            pos->x = entity.x();
            pos->y = entity.y();
        } else {
            ecsEntity = rltk::create_entity(entity.id());
            ecsEntity->assign(position_t{entity.x(), entity.y()});
            ecsEntity->assign(renderable_t{static_cast<char>(entity.icon()), rltk::colors::AZURE});
        }
    }
}

PlayScene::map_render_system::map_render_system(data::Level::Ptr &level)
    : mLevel(level)
{
}

void PlayScene::map_render_system::update(const double duration_ms)
{
    auto logger = spdlog::get("default");
    auto term = rltk::term(PlayScene::CONSOLE_LAYER);
    if (mLevel) {
        for (int y = 0; y < mLevel->size().height; ++y) {
            for (int x = 0; x < mLevel->size().width; ++x) {
                auto tile = mLevel->at(x, y);
                term->set_char(x + 1, y + 1, rltk::vchar{ tile.tile, rltk::colors::WHITE, rltk::colors::BLACK });
            }
        }
    }
}

PlayScene::entities_render_system::entities_render_system()
{

}

void PlayScene::entities_render_system::update(const double duration_ms)
{
    auto term = rltk::term(PlayScene::CONSOLE_LAYER);
    rltk::each<position_t, renderable_t>([&](rltk::entity_t &e, position_t &pos, renderable_t &render) {
        term->set_char(pos.x + 1, pos.y + 1, rltk::vchar{ render.glyph, render.fg, render.bg });
    });
}

PlayScene::player_system::player_system(AsioNetworkClient &network)
    : mNetwork(network)
{
    subscribe_mbox<rltk::key_pressed_t>();
}

void PlayScene::player_system::update(const double duration_ms)
{
    auto logger = spdlog::get("default");

    int move = 0;
    // Loop through the keyboard input list
    std::queue<rltk::key_pressed_t> * messages = mbox<rltk::key_pressed_t>();
    while (!messages->empty()) {
        rltk::key_pressed_t e = messages->front();
        messages->pop();

        if (e.event.key.code == sf::Keyboard::Up)
            move = 1;
        else if (e.event.key.code == sf::Keyboard::Right)
            move = 2;
        else if (e.event.key.code == sf::Keyboard::Down)
            move = 3;
        else if (e.event.key.code == sf::Keyboard::Left)
            move = 4;
        logger->debug("Key Pressed {}", move);
    }

    if (move) {
        logger->debug("Player Move {}", move);
        net::PlayerMove pm;
        pm.set_direction(net::PlayerMove_Direction(move));
        mNetwork.send(pm);
    }
}
