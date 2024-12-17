#pragma once
#include <array>
#include <cstdint>
#include <iostream>

class Kernel {
public:
    
private:
    struct Dimension {
        uint64_t width_;
        uint64_t height_;
    };
    struct Strategy {
        // временная заглушка для стратегии
    };
    bool type_;  // тип игрока
    Dimension dimension_;  // размерность поля
    Strategy strategy_;  // выбранная стратегия
    std::array<uint64_t, 4> ships_;  // количество кораблей
    bool win_ = false;  // выиграна ли игра
};

void Hello();