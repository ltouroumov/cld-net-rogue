//
// Created by ldavid on 5/12/17.
//

#include "ClientHandler.hpp"
#include "bintools.hpp"
#include "game.pb.h"
#include "GameLiftManager.hpp"

namespace pb = google::protobuf;

ClientHandler::ClientHandler(tcp::socket socket, Pool &clientPool, world::Handler &worldHandler)
    : mSocket(std::move(socket))
      , mClientPool(clientPool)
      , mWorldHandler(worldHandler)
{
    mDispatcher.wire<net::Connect>(std::bind(&ClientHandler::HandleConnect, this, std::placeholders::_1));
    mDispatcher.wire<net::PlayerMove>(std::bind(&ClientHandler::HandlePlayerMove, this, std::placeholders::_1));
}

ClientHandler::~ClientHandler()
{
    auto logger = spdlog::get(LOGGER_NAME);
    logger->debug("Client Session Destroyed");
}

void ClientHandler::Start()
{
    mClientPool.insert(shared_from_this());
    DoRead();
}

void ClientHandler::Stop()
{
    mSocket.close();
    OnSocketClose();
}

void ClientHandler::Send(pb::Message &msg)
{
    auto logger = spdlog::get("main");
    std::error_code ec;

    std::array<uint8_t, 4> header;
    std::vector<uint8_t> body;

    uint16_t type = mPacketData.TypeFromPacket(msg);
    size_t size = msg.ByteSizeLong();

    write_uint16(header.data(), 0, type);
    write_uint16(header.data(), 2, size);

    body.resize(size, 0);
    msg.SerializeToArray(body.data(), body.size());

    // logger->debug("Sending packet {} (len {})", type, size);

    mSocket.send(asio::buffer(header), 0, ec);
    mSocket.send(asio::buffer(body), 0, ec);
}

void ClientHandler::DoRead()
{
    using namespace std::placeholders;

    mSocket.async_read_some(
        mPacket.HeaderBuffer(),
        std::bind(&ClientHandler::OnReadHeader, this, _1, _2)
    );
}

void ClientHandler::OnReadHeader(std::error_code ec, std::size_t len)
{
    using namespace std::placeholders;
    auto logger = spdlog::get(LOGGER_NAME);

    if (!ec) {
        if (mPacket.ParseHeader()) {
            logger->debug("Got header, type: {:d}, length: {:d}", mPacket.Type(), mPacket.Size());
            mSocket.async_read_some(mPacket.BodyBuffer(), std::bind(&ClientHandler::OnReadBody, this, _1, _2));
        } else {
            logger->warn("Malformed Packet Header! Closed Connection.");
            mSocket.close();
            OnSocketClose();
        }
    } else if (ec == asio::error::eof) {
        OnSocketClose();
    } else {
        logger->error("Socket Error: {:s}", ec.message());
    }
}

void ClientHandler::OnReadBody(std::error_code ec, std::size_t len)
{
    using namespace std::placeholders;
    auto mLogger = spdlog::get(LOGGER_NAME);

    if (!ec) {
        mLogger->debug("Got body, read: {:d}, expected: {:d}", len, mPacket.Size());
        if (len != mPacket.Size()) {
            mLogger->warn("Read {:s} bytes, expected {:d}", len, mPacket.Size());
        } else {
            std::shared_ptr<pb::Message> msg = std::shared_ptr<pb::Message>(mPacketData.PacketFromType(mPacket.Type()));
            if (msg) {
                msg->ParseFromArray(mPacket.Body(), mPacket.Size());
                mDispatcher.dispatch(*msg);
            } else {
                mLogger->warn("Unrecognized Packet Type: {:d}", mPacket.Type());
            }
        }

        DoRead();
    } else if (ec == asio::error::eof || ec == asio::error::connection_reset) {
        mLogger->debug("Client Socket Closed");
        OnSocketClose();
    } else {
        // Express displeasure at the console
        mLogger->error("Socket error: {}", ec.message());
    }
}

void ClientHandler::OnSocketClose()
{
    auto logger = spdlog::get(LOGGER_NAME);
    logger->info("Client Socket Closed");
    mClientPool.erase(shared_from_this());
    mWorldHandler.leave(shared_from_this());
}

void ClientHandler::HandleConnect(net::Connect &msg)
{
    auto mLogger = spdlog::get(LOGGER_NAME);
    mLogger->debug("Player connect");

    if (GGameLiftManager->AcceptPlayerSession(shared_from_this(), msg.playersessionid())) {
        net::ConnectResponse crsp;
        crsp.set_accepted(true);
        Send(crsp);

        mPlayerEntity = mWorldHandler.enter(shared_from_this());
    } else {
        Stop();
    }
}

void ClientHandler::HandlePlayerMove(net::PlayerMove &msg)
{
    world::Entity::Controller* controller = mPlayerEntity->controller();
    world::PlayerController* playerController = (world::PlayerController*)controller;
    playerController->move(msg.direction());
}

