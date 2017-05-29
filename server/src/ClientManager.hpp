//
// Created by ldavid on 5/12/17.
//

#include <asio.hpp>
#include <set>
#include <spdlog/spdlog.h>
#include "Config.hpp"
#include "ClientHandler.hpp"
#include "world/Handler.hpp"

#ifndef PREZ_GAME_CLIENTMANAGER_HPP
#define PREZ_GAME_CLIENTMANAGER_HPP

using asio::ip::tcp;

class ClientManager
{
public:
    ClientManager(asio::io_service &io_service, const tcp::endpoint &endpoint, world::Handler &worldHandler);

private:
    void DoAccept();

    world::Handler &mWorldHandler;
    tcp::socket mSocket;
    tcp::acceptor mAcceptor;
    ClientHandler::Pool mClients;
};

#endif //PREZ_GAME_CLIENTMANAGER_HPP
