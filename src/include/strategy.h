#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include "utils.h"

class Strategy {
public:
    bool SetType(uint8_t type);
private:
    std::array<uint8_t, 2> types_ = {0, 1};  // 0 - orderred, 1 - custom
    uint8_t type_ = 1;
};
