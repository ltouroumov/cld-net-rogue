//
// Created by ldavid on 5/12/17.
//

#include <memory>
#include <set>
#ifdef HAS_GAMELIFT
#include <aws/gamelift/server/model/GameSession.h>
#endif
#include <spdlog/spdlog.h>
#include "ClientHandler.hpp"

#ifndef PREZ_GAME_GAMELIFTMANAGER_HPP
#define PREZ_GAME_GAMELIFTMANAGER_HPP

class GameLiftManager
{
public:
    GameLiftManager();

    bool InitializeGameLift(int listenPort);

    void FinalizeGameLift();

    bool AcceptPlayerSession(std::shared_ptr <ClientHandler> psess, const std::string &playerSessionId);

    void RemovePlayerSession(std::shared_ptr <ClientHandler> psess, const std::string &playerSessionId);

#ifdef HAS_GAMELIFT
    void OnStartGameSession(Aws::GameLift::Server::Model::GameSession);
#else
    void OnStartGameSession();
#endif

    void OnProcessTerminate();

    bool OnHealthCheck()
    { return mActivated; }

private:
    bool mActivated;
    char** mLogPaths;
    std::set<std::shared_ptr<ClientHandler>> mClientSessions;
};

extern std::unique_ptr<GameLiftManager> GGameLiftManager;

#endif //PREZ_GAME_GAMELIFTMANAGER_HPP