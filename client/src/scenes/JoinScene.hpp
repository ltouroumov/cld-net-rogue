//
// Created by ldavid on 5/22/17.
//

#include <string>
#include "../util/Scene.hpp"
#include "../AsioNetworkClient.hpp"
#include <aws/gamelift/GameLiftClient.h>
#include <aws/core/utils/Outcome.h>
#include <aws/gamelift/model/CreatePlayerSessionRequest.h>
#include <aws/gamelift/model/CreatePlayerSessionResult.h>

#ifndef PREZ_GAME_JOINSCENE_HPP
#define PREZ_GAME_JOINSCENE_HPP

class JoinScene : public Scene
{
public:
    JoinScene(Director *director, const std::string& gameSessionId, const std::string& pseudo);

    void enter() override;

    void tick(double delta) override;

    void leave() override;

private:
    void awsPlayerSessionCreated(const Aws::GameLift::GameLiftClient*, const Aws::GameLift::Model::CreatePlayerSessionRequest&, const Aws::GameLift::Model::CreatePlayerSessionOutcome&, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&);
    void onConnectResponse(net::ConnectResponse& response);

    Aws::GameLift::Model::PlayerSession mPlayerSession;
    std::string mGameSessionId;
    std::string mPseudo;

    std::shared_ptr<AsioNetworkClient> mNetwork;
    // Handler IDS
    size_t _crHandler;
};


#endif //PREZ_GAME_JOINSCENE_HPP
