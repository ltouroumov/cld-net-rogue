//
// Created by ldavid on 5/22/17.
//

#include "../util/Scene.hpp"
#include "../AsioNetworkClient.hpp"
#include <rltk/rltk.hpp>
#include <aws/core/utils/Outcome.h>
#include <aws/gamelift/model/DescribeGameSessionsRequest.h>
#include <aws/gamelift/model/DescribeGameSessionsResult.h>
#include <aws/gamelift/model/CreateGameSessionRequest.h>
#include <aws/gamelift/model/CreateGameSessionResult.h>
#include <aws/gamelift/GameLiftClient.h>
#include <data/Level.hpp>

#ifndef PREZ_GAME_PLAYSCENE_HPP
#define PREZ_GAME_PLAYSCENE_HPP

class PlayScene : public Scene
{
public:
    PlayScene(Director *director, Aws::GameLift::Model::PlayerSession &, std::shared_ptr<AsioNetworkClient> &);

    void tick(double delta) override;

    void enter() override;

    void leave() override;

    void attached() override;

    void detached() override;

private:
    struct position_t
    {
        position_t()
        {}

        position_t(const int X, const int Y)
            : x(X), y(Y)
        {}

        int x, y;
    };

    struct renderable_t
    {
        renderable_t()
        {}

        renderable_t(const char Glyph, const rltk::color_t foreground)
            : glyph(Glyph), fg(foreground)
        {}

        int glyph;
        rltk::color_t fg = rltk::colors::WHITE;
        rltk::color_t bg = rltk::colors::BLACK;
    };

    struct map_render_system : public rltk::base_system
    {
        map_render_system(data::Level::Ptr &level);

        void update(const double duration_ms) override;

    private:
        data::Level::Ptr &mLevel;
    };

    struct entities_render_system : public rltk::base_system
    {
        entities_render_system();

        void update(const double duration_ms) override;
    };

    struct player_system : public rltk::base_system
    {
        player_system(AsioNetworkClient& network);

        void update(const double duration_ms) override;

    private:
        AsioNetworkClient& mNetwork;
    };

    void onTick(net::Tick &tick);

    Aws::GameLift::Model::PlayerSession mPlayerSession;
    std::shared_ptr<AsioNetworkClient> &mNetwork;
    data::Level::Ptr mLevel;

    size_t mTickId;

    size_t _tickHandler;

    static int CONSOLE_LAYER;
};

#endif //PREZ_GAME_PLAYSCENE_HPP
