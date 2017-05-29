//
// Created by ldavid on 5/12/17.
//

#include <iostream>
#include "PacketHandler.hpp"
#include "bintools.hpp"

/**
 * Header Structure
 *
 * - 2bytes of packet_type
 * - 2bytes of body_length
 *
 * @return header buffer
 */
bool PacketHandler::ParseHeader()
{
    mType = read_uint16(mHeader.data(), 0);
    mSize = read_uint16(mHeader.data(), 2);
    mBody.resize(mSize, 0);

    return true;
}

uint16_t PacketHandler::Size()
{
    return mSize;
}

uint16_t PacketHandler::Type()
{
    return mType;
}
