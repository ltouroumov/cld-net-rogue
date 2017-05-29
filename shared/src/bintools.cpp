//
// Created by ldavid on 5/12/17.
//

#include <cstring>
#include <cstdio>
#include <netinet/in.h>
#include "bintools.hpp"

std::string to_hex(const uint8_t* buf, size_t len) {
    char* hex = new char[len * 2 + 1];
    memset(hex, '0', len * 2);
    hex[len * 2] = '\0';

    for (size_t n = 0; n < len; ++n) {
        sprintf(hex + n * 2, "%02x", buf[n]);
    }

    return std::string(hex);
}

uint16_t read_uint16(uint8_t* buf, size_t index) {
    uint16_t beval;
    uint8_t* bval = (uint8_t*)&beval;

    memcpy(bval, buf + index, sizeof(uint16_t));

    return ntohs(beval);
}

void write_uint16(uint8_t* buf, size_t index, const uint16_t val) {
    uint8_t* b = buf + index;
    uint16_t beval = htons(val);
    uint8_t* vbuf = (uint8_t *)&beval;
    b[0] = vbuf[0];
    b[1] = vbuf[1];
}