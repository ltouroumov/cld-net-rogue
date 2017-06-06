//
// Created by ldavid on 5/22/17.
//

#include <spdlog/spdlog.h>
#include "JoinScene.hpp"
#include "../util/Director.hpp"
#include "../static.hpp"
#include "PlayScene.hpp"
#include "game.pb.h"

JoinScene::JoinScene(Director *director, const std::string& gameSessionId, const std::string& pseudo)
    : Scene(director), mGameSessionId(gameSessionId), mPseudo(pseudo), mNetwork(new AsioNetworkClient())
{}

void JoinScene::enter()
{
    using namespace std::placeholders;

    Aws::GameLift::Model::CreatePlayerSessionRequest request;
    request.SetPlayerId(mPseudo);
    request.SetGameSessionId(mGameSessionId);

    glClient->CreatePlayerSessionAsync(request, std::bind(&JoinScene::awsPlayerSessionCreated, this, _1, _2, _3, _4));

    _crHandler = mNetwork->dispatcher().wire<net::ConnectResponse>(std::bind(&JoinScene::onConnectResponse, this, _1));
}

void JoinScene::leave()
{
    mNetwork->dispatcher().unwire(_crHandler);
}

void JoinScene::tick(double delta)
{
    // It's a loading screen
}

void JoinScene::awsPlayerSessionCreated(
    const Aws::GameLift::GameLiftClient *,
    const Aws::GameLift::Model::CreatePlayerSessionRequest &,
    const Aws::GameLift::Model::CreatePlayerSessionOutcome &outcome,
    const std::shared_ptr<const Aws::Client::AsyncCallerContext> &
) {
    auto logger = spdlog::get("default");

    if (outcome.IsSuccess()) {
        logger->info("Player Session Created");
        mPlayerSession = outcome.GetResult().GetPlayerSession();
        bool success = mNetwork->connect(
            mPlayerSession.GetIpAddress(),
            mPlayerSession.GetPort()
        );

        if (!success) {
            director()->pop();
        } else {
            net::Connect conn;
            conn.set_playersessionid(mPlayerSession.GetPlayerSessionId());
            conn.set_playername(mPlayerSession.GetPlayerId());
            mNetwork->send(conn);
        }
    } else {
        logger->error("Failed to create player session");
        director()->pop();
    }
}

void JoinScene::onConnectResponse(net::ConnectResponse &response)
{
    auto logger = spdlog::get("default");

    if (response.accepted()) {
        director()->replace<PlayScene>(mPlayerSession, mNetwork);
    } else {
        director()->pop();
        logger->warn("Got no for connect");
    }
}
