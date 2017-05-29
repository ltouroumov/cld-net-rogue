//
// Created by ldavid on 5/12/17.
//

#include <memory>
#include <set>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include "PacketData.hpp"
#include "Config.hpp"
#include "PacketHandler.hpp"
#include "PacketDispatcher.hpp"
#include "world/Handler.hpp"

#ifndef PREZ_GAME_CLIENTSESSION_HPP
#define PREZ_GAME_CLIENTSESSION_HPP

using asio::ip::tcp;


class ClientHandler : public std::enable_shared_from_this<ClientHandler>
{
public:
    typedef std::set<std::shared_ptr<ClientHandler>> Pool;

    ClientHandler(tcp::socket socket, Pool &pool, world::Handler &world);

    ~ClientHandler();

    void Start();

    void Stop();

    PacketDispatcher &GetDispatcher()
    { return mDispatcher; }

    void Send(pb::Message &msg);

private:
    void DoRead();

    void OnReadHeader(std::error_code ec, std::size_t len);

    void OnReadBody(std::error_code ec, std::size_t len);

    void OnSocketClose();

    void HandleConnect(net::Connect &msg);
    void HandlePlayerMove(net::PlayerMove &msg);

    world::Handler &mWorldHandler;
    Pool &mClientPool;
    tcp::socket mSocket;
    PacketHandler mPacket;
    PacketData mPacketData;
    PacketDispatcher mDispatcher;
    std::shared_ptr<world::Entity> mPlayerEntity;
};


#endif //PREZ_GAME_CLIENTSESSION_HPP
