#pragma once
#include <array>
#include <cstdint>
#include <iostream>

class Kernel {
public:
    Kernel(bool type);

    bool SetWidth(uint64_t width);
    bool SetHeight(uint64_t height);
    bool SetCount(uint8_t type, uint64_t count);

private:
    struct Dimension {
        uint64_t width_;
        uint64_t height_;
    };
    struct Strategy {
        // временная заглушка для стратегии
    };
    bool type_;  // тип игрока: 0 - slave; 1 - master
    Dimension dimension_;  // размерность поля
    Strategy strategy_;  // выбранная стратегия
    std::array<uint64_t, 4> ships_;  // количество кораблей
    bool win_ = false;  // выиграна ли игра
};
