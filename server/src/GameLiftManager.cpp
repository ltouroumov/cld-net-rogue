//
// Created by ldavid on 5/12/17.
//

#include "GameLiftManager.hpp"

#ifdef HAS_GAMELIFT
#include <aws/gamelift/server/GameLiftServerAPI.h>
#endif

std::unique_ptr<GameLiftManager> GGameLiftManager;

GameLiftManager::GameLiftManager()
: mActivated(false)
{

}

bool GameLiftManager::InitializeGameLift(int listenPort)
{
#ifdef HAS_GAMELIFT
    auto mLogger = spdlog::get("default");
    using namespace std::placeholders;

    auto initOutcome = Aws::GameLift::Server::InitSDK();

    if (!initOutcome.IsSuccess())
        return false;

    std::vector<std::string> logPaths;
    logPaths.push_back("./logs/main.log");
    logPaths.push_back("./logs/shared.log");

    auto processReadyParameter = Aws::GameLift::Server::ProcessParameters(
        std::bind(&GameLiftManager::OnStartGameSession, this, _1),
        std::bind(&GameLiftManager::OnProcessTerminate, this),
        std::bind(&GameLiftManager::OnHealthCheck, this),
        listenPort,
        Aws::GameLift::Server::LogParameters(logPaths)
    );

    auto readyOutcome = ProcessReady(processReadyParameter);

    if (!readyOutcome.IsSuccess())
        return false;

    mActivated = true;

    mLogger->info("ProcessReady Success");
#endif
    return true;
}

void GameLiftManager::FinalizeGameLift()
{
#ifdef HAS_GAMELIFT
    Aws::GameLift::Server::Destroy();
#endif
}

bool GameLiftManager::AcceptPlayerSession(std::shared_ptr<ClientHandler> psess, const std::string &playerSessionId)
{
#ifdef HAS_GAMELIFT
    auto mLogger = spdlog::get("default");
    auto outcome = Aws::GameLift::Server::AcceptPlayerSession(playerSessionId);

    if (outcome.IsSuccess())
    {
        mLogger->info("AcceptPlayerSession: {}", playerSessionId);
        mClientSessions.insert(psess);
        return true;
    }

    mLogger->error("AcceptPlayerSession Fail: {:s}", outcome.GetError().GetErrorMessage());

    return false;
#else
    return true;
#endif
}

void GameLiftManager::RemovePlayerSession(std::shared_ptr<ClientHandler> psess, const std::string &playerSessionId)
{
#ifdef HAS_GAMELIFT
    auto mLogger = spdlog::get("default");
    auto outcome = Aws::GameLift::Server::RemovePlayerSession(playerSessionId);
    if (outcome.IsSuccess())
    {
        mLogger->debug("RemovedPlayerSession: {:s}", playerSessionId);
        mClientSessions.erase(psess);
    }
    else
    {
        mLogger->error("RemovePlayerSession Fail: {:s}",
                       outcome.GetError().GetErrorName());
    }
#endif
}

#ifdef HAS_GAMELIFT
void GameLiftManager::OnStartGameSession(Aws::GameLift::Server::Model::GameSession)
{
    Aws::GameLift::Server::ActivateGameSession();

    auto mLogger = spdlog::get("default");
    mLogger->info("OnStartGameSession Success");
}
#else
void GameLiftManager::OnStartGameSession()
{
}
#endif

void GameLiftManager::OnProcessTerminate()
{
#ifdef HAS_GAMELIFT
    // game-specific tasks required to gracefully shut down a game session,
    // such as notifying players, preserving game state data, and other cleanup

    Aws::GameLift::Server::TerminateGameSession();
    Aws::GameLift::Server::ProcessEnding();

    mActivated = false;

    auto mLogger = spdlog::get("default");
    mLogger->info("OnProcessTerminate Success");
#endif

    exit(0);
}

