//
// Created by ldavid on 5/17/17.
//

#include "../util/Scene.hpp"
#include <rltk/rltk.hpp>
#include <aws/core/utils/Outcome.h>
#include <aws/gamelift/model/DescribeGameSessionsRequest.h>
#include <aws/gamelift/model/DescribeGameSessionsResult.h>
#include <aws/gamelift/model/CreateGameSessionRequest.h>
#include <aws/gamelift/model/CreateGameSessionResult.h>
#include <aws/gamelift/GameLiftClient.h>

#ifndef PREZ_GAME_MENUSCENE_HPP
#define PREZ_GAME_MENUSCENE_HPP

class MenuScene : public Scene
{
public:
    MenuScene(Director*);

    void tick(double delta) override;

    void enter() override;
    void leave() override;

    void attached() override;
    void detached() override;

private:
    void awsSessionsResponse(const Aws::GameLift::GameLiftClient*, const Aws::GameLift::Model::DescribeGameSessionsRequest&, const Aws::GameLift::Model::DescribeGameSessionsOutcome&, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&);
    void awsSessionCreated(const Aws::GameLift::GameLiftClient*, const Aws::GameLift::Model::CreateGameSessionRequest&, const Aws::GameLift::Model::CreateGameSessionOutcome&, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&);

    std::vector<std::string> mSessions;
    std::string mSelectedSession;

    double mLastUpdate;
    bool mApiRequest = false;
    bool mApiError = false;

    void resize_bg(rltk::layer_t * l, int w, int h);
    void draw_bg(rltk::layer_t * l, sf::RenderTexture &window);

    const size_t mPseudoSize = 128;
    char* mPseudo = nullptr;

    const int BACKGROUND_LAYER = 1;
    const int UPDATE_DELTA = 5 * 100;
};


#endif //PREZ_GAME_MENUSCENE_HPP
