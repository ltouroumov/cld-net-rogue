//
// Created by ldavid on 5/22/17.
//

#include <asio.hpp>
#include <thread>
#include <PacketDispatcher.hpp>
#include "PacketData.hpp"

#ifndef PREZ_GAME_NETWORKCLIENT_HPP
#define PREZ_GAME_NETWORKCLIENT_HPP

using asio::ip::tcp;

class AsioNetworkClient
{
public:
    AsioNetworkClient();

    bool connect(const std::string& ipAddress, int port);

    void disconnect();

    void send(pb::Message& message);

    PacketDispatcher& dispatcher();

private:
    void receive();

    asio::io_service mIoService;
    tcp::resolver mResolver;
    tcp::socket mSocket;
    PacketData mPacketData;
    std::thread mReceiver;
    PacketDispatcher mDispatcher;
};

#endif //PREZ_GAME_NETWORKCLIENT_HPP
