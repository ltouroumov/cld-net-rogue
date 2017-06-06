//
// Created by ldavid on 5/12/17.
//

#include "ClientManager.hpp"

ClientManager::ClientManager(asio::io_service &io_service, const tcp::endpoint &endpoint, world::Handler &worldHandler)
    : mAcceptor(io_service, endpoint)
      , mSocket(io_service)
      , mWorldHandler(worldHandler)
{
    DoAccept();
}

void ClientManager::DoAccept()
{
    mAcceptor.async_accept(mSocket, [this](std::error_code ec) {
        if (!ec) {
            auto logger = spdlog::get("default");
            logger->info("Accepted Client");

            std::make_shared<ClientHandler>(std::move(mSocket), mClients, mWorldHandler)->Start();
        }

        DoAccept();
    });
}