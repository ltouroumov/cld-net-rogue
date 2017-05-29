//
// Created by ldavid on 5/12/17.
//

#include <cstdint>
#include <cstddef>
#include <string>

#ifndef PREZ_GAME_BINTOOLS_HPP
#define PREZ_GAME_BINTOOLS_HPP

std::string to_hex(const uint8_t* buf, size_t len);
uint16_t read_uint16(uint8_t* buf, size_t index);
void write_uint16(uint8_t* buf, size_t index, const uint16_t val);

#endif //PREZ_GAME_BINTOOLS_HPP
