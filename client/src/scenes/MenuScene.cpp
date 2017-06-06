//
// Created by ldavid on 5/17/17.
//

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "MenuScene.hpp"
#include "../util/Director.hpp"
#include "JoinScene.hpp"
#include "../static.hpp"

MenuScene::MenuScene(Director* director, ClientConfig& config)
    : Scene(director), mConfig(config)
{
    rltk::register_texture("assets/background_image.png", "backdrop");
    mPseudo = new char[mPseudoSize];
    memset(mPseudo, 0, mPseudoSize);
}

void MenuScene::tick(double delta)
{
    auto logger = spdlog::get("default");

    ImGui::Begin("Main Menu");

    ImGui::InputText("Pseudo", mPseudo, mPseudoSize);

    ImGui::Separator();

    ImGui::Text("Open Game Sessions");

    for (size_t n = 0; n < mSessions.size(); ++n) {
        std::string& sess_id = mSessions.at(n);
        ImGui::Bullet();
        if (ImGui::Selectable(sess_id.c_str(), mSelectedSession == sess_id)) {
            mSelectedSession = sess_id;
        }
    }

    ImGui::Spacing();

    if (mSelectedSession.size() > 0 && strlen(mPseudo) > 0) {
        if (ImGui::Button("Join")) {
            director()->push<JoinScene>(mSelectedSession, mPseudo);
        }
        ImGui::SameLine();
    }

    if (ImGui::Button("New Session!")) {
        using namespace std::placeholders;

        Aws::GameLift::Model::CreateGameSessionRequest request;
#ifdef LOCAL_GAMELIFT
        request.SetFleetId("fleet-test");
#else
        request.SetAliasId(mConfig.getAliasId());
#endif
        request.SetMaximumPlayerSessionCount(16);
        glClient->CreateGameSessionAsync(request, std::bind(&MenuScene::awsSessionCreated, this, _1, _2, _3, _4));
    }

    ImGui::End();

    mLastUpdate += delta;
    if (mLastUpdate > UPDATE_DELTA && !mApiError && !mApiRequest) {
        logger->debug("Updating Sessions");

        using namespace std::placeholders;
        Aws::GameLift::Model::DescribeGameSessionsRequest request;
#ifdef LOCAL_GAMELIFT
        request.SetFleetId("fleet-test");
#else
        request.SetAliasId(mConfig.getAliasId());
#endif
        mApiRequest = true;
        glClient->DescribeGameSessionsAsync(request, std::bind(&MenuScene::awsSessionsResponse, this, _1, _2, _3, _4));
        mLastUpdate = 0;
    }
}

void MenuScene::awsSessionsResponse(
    const Aws::GameLift::GameLiftClient* client,
    const Aws::GameLift::Model::DescribeGameSessionsRequest& request,
    const Aws::GameLift::Model::DescribeGameSessionsOutcome& outcome,
    const std::shared_ptr<const Aws::Client::AsyncCallerContext>& ctx
) {
    auto logger = spdlog::get("default");

    if (outcome.IsSuccess()) {
        const auto &sessions = outcome.GetResult().GetGameSessions();
        logger->debug("Got {:d} Game Sessions", sessions.size());
        mSessions.clear();
        for (const auto &session : sessions) {
            if (session.GetStatus() == Aws::GameLift::Model::GameSessionStatus::ACTIVE) {
                const Aws::String &str = session.GetGameSessionId();
                mSessions.emplace_back(str.c_str());
            }
        }
    } else {
        logger->error("Failed to update sessions: {}", outcome.GetError().GetMessage());
        mApiError = true;
    }
    mApiRequest = false;
}

void MenuScene::awsSessionCreated(
    const Aws::GameLift::GameLiftClient*,
    const Aws::GameLift::Model::CreateGameSessionRequest&,
    const Aws::GameLift::Model::CreateGameSessionOutcome& outcome,
    const std::shared_ptr<const Aws::Client::AsyncCallerContext>&
) {
    auto logger = spdlog::get("default");

    if (outcome.IsSuccess()) {
        logger->info("Session Created");
    } else {
        logger->error("Failed to create session: {}", outcome.GetError().GetMessage());
    }
}

void MenuScene::resize_bg(rltk::layer_t * l, int w, int h) {
    // Use the whole window
    l->w = w;
    l->h = h;
}

void MenuScene::draw_bg(rltk::layer_t * l, sf::RenderTexture &window) {
    sf::Texture * bg = rltk::get_texture("backdrop");
    sf::Sprite backdrop(*bg);
    window.draw(backdrop);
}

void MenuScene::enter()
{
    rltk::layer(BACKGROUND_LAYER)->draw = true;
}

void MenuScene::leave()
{
    rltk::layer(BACKGROUND_LAYER)->draw = false;
}

void MenuScene::attached()
{
    using namespace std::placeholders;

    rltk::gui->add_owner_layer(
        BACKGROUND_LAYER,
        0, 0, 1024, 768,
        std::bind(&MenuScene::resize_bg, this, _1, _2, _3),
        std::bind(&MenuScene::draw_bg, this, _1, _2)
    );
}

void MenuScene::detached()
{
    rltk::gui->delete_layer(BACKGROUND_LAYER);
}
