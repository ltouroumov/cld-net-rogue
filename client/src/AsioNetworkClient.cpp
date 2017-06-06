//
// Created by ldavid on 5/22/17.
//

#include <spdlog/spdlog.h>
#include "AsioNetworkClient.hpp"
#include "bintools.hpp"

AsioNetworkClient::AsioNetworkClient()
    : mResolver(mIoService)
    , mSocket(mIoService)
{

}

bool AsioNetworkClient::connect(const std::string &ipAddress, int port)
{
    auto logger = spdlog::get("default");

    try {
        logger->info("Connecting to {}:{}", ipAddress, port);

        std::error_code ec;
        // std::string portStr = fmt::format("%d", port);
        // tcp::resolver::query query(ipAddress, portStr);
        // tcp::resolver::iterator endpoint_iterator = mResolver.resolve(query);
        tcp::endpoint endpoint(asio::ip::address::from_string(ipAddress), port);

        mSocket.connect(endpoint, ec);

        if (ec) {
            logger->error("Connect error: {}", ec.message());
            return false;
        }

        std::thread receiver(std::bind(&AsioNetworkClient::receive, this));
        mReceiver = std::move(receiver);
        logger->info("Connection established");
        return true;
    } catch (const std::exception& ex) {
        logger->error("Connect Failed: {}", ex.what());
        return false;
    }
}

void AsioNetworkClient::disconnect()
{
    if (mSocket.is_open()) {
        mSocket.close();
    }
}

void AsioNetworkClient::send(pb::Message &message)
{
    auto logger = spdlog::get("default");
    std::error_code ec;

    std::array<uint8_t, 4> header;
    std::vector<uint8_t> body;

    uint16_t type = mPacketData.TypeFromPacket(message);
    size_t size = message.ByteSizeLong();

    write_uint16(header.data(), 0, type);
    write_uint16(header.data(), 2, size);

    body.resize(size, 0);
    message.SerializeToArray(body.data(), body.size());

    logger->debug("Sending packet {} (len {})", type, size);

    mSocket.send(asio::buffer(header), 0, ec);
    mSocket.send(asio::buffer(body), 0, ec);
}

void AsioNetworkClient::receive()
{
    auto logger = spdlog::get("default");

    logger->info("Receive loop started");

    std::error_code ec;

    std::array<uint8_t, 4> header;
    std::vector<uint8_t> body;

    while (true) {
        mSocket.receive(asio::buffer(header.data(), header.size()), 0, ec);

        if (ec == asio::error::eof) {
            //Socket closed
            logger->info("Socket closed by remote host");
            return;
        }

        uint16_t type = read_uint16(header.data(), 0);
        uint16_t size = read_uint16(header.data(), 2);

        body.resize(size, 0);
        mSocket.receive(asio::buffer(body.data(), body.size()), 0, ec);
        if (ec) {
            logger->error("Socket read error {}", ec.message().c_str());
            return;
        }

        pb::Message* msg = mPacketData.PacketFromType(type);
        if (!msg) {
            // Unrecognized message type
            logger->error("Unrecognized packet type");
            continue;
        }
        msg->ParseFromArray(body.data(), body.size());

        // logger->debug("Got Packet {}", msg->GetDescriptor()->name());

        mDispatcher.dispatch(*msg);
    }
}

PacketDispatcher &AsioNetworkClient::dispatcher()
{
    return mDispatcher;
}
