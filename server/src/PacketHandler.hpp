//
// Created by ldavid on 5/12/17.
//

#include <array>
#include <asio.hpp>

#ifndef PREZ_GAME_PACKETHANDLER_HPP
#define PREZ_GAME_PACKETHANDLER_HPP

class PacketHandler
{
public:

    auto HeaderBuffer()
    {
        return asio::buffer(mHeader.data(), mHeader.size());
    }

    auto BodyBuffer()
    {
        return asio::buffer(mBody.data(), mBody.size());
    }

    uint16_t Type();
    uint16_t Size();
    const uint8_t* Body()
    {
        return mBody.data();
    }

    bool ParseHeader();

private:
    uint16_t mType;
    uint16_t mSize;

    std::array<uint8_t, 4> mHeader;
    std::vector<uint8_t> mBody;
};

#endif //PREZ_GAME_PACKETHANDLER_HPP
