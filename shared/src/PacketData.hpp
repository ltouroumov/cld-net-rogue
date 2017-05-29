//
// Created by ldavid on 5/16/17.
//

#include "game.pb.h"

#ifndef PREZ_GAME_PACKETDATA_HPP
#define PREZ_GAME_PACKETDATA_HPP

namespace pb = ::google::protobuf;

class PacketData
{
public:
    typedef std::function<pb::Message*()> packet_factory;

    static void Initialize();

    pb::Message* PacketFromType(uint16_t type);
    uint16_t TypeFromPacket(const pb::Message& msg);

private:
    template<typename PacketType>
    class packet_builder {
    public:
        pb::Message* operator()() {
            return new PacketType();
        }
    };

    template<typename PacketType>
    static void AddType(int idx) {
        const auto& type_id = typeid(PacketType).hash_code();

        sPacketTypes.insert(std::make_pair(idx, packet_builder<PacketType>()));
        sPacketIds.insert(std::make_pair(type_id, idx));
    }

    static std::map<uint16_t , packet_factory> sPacketTypes;
    static std::map<size_t, uint16_t> sPacketIds;
};


#endif //PREZ_GAME_PACKETDATA_HPP
