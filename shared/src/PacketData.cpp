//
// Created by ldavid on 5/16/17.
//

#include <spdlog/spdlog.h>
#include "PacketData.hpp"

std::map<uint16_t , PacketData::packet_factory> PacketData::sPacketTypes;
std::map<size_t, uint16_t> PacketData::sPacketIds;

void PacketData::Initialize()
{
    AddType<net::Connect>(1);
    AddType<net::ConnectResponse>(2);
    AddType<net::Tick>(3);
    AddType<net::PlayerMove>(16);
}

pb::Message* PacketData::PacketFromType(uint16_t type)
{
    auto it = sPacketTypes.find(type);

    if (it != sPacketTypes.end()) {
        auto pair = *it;
        return pair.second();
    } else {
        return nullptr;
    }

    /*
    switch (type) {
        case 1:
            return new Connect();
        default:
            return nullptr;
    }
     */
}

uint16_t PacketData::TypeFromPacket(const pb::Message& msg)
{
    // auto logger = spdlog::get("default");
    const std::type_info& msgType = typeid(msg);

    auto it = sPacketIds.find(msgType.hash_code());

    if (it != sPacketIds.end()) {
        return it->second;
    } else {
        return 0xFFFF;
    }

    /*
    if (msgType == typeid(Connect)) {
        return 1;
    } else {
        return 0xFFFF;
    }
     */
}

