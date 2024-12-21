#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include "map.h"
#include "strategy.h"

class Kernel {
public:
    Kernel(bool type);

    bool SetWidth(uint64_t width);
    bool SetHeight(uint64_t height);
    bool SetCount(uint8_t type, uint64_t count);
    void SetOrderedStrategy();
    void SetCustomStrategy();

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    uint64_t GetCount(uint8_t type) const;
    const Map& GetMap() const ;
    Coords GetMove() const;

    bool IsReady() const;
    bool IsStarted() const;
    bool IsLoose() const;
    bool IsStrategySet() const;

    void Start();
    void HitShip();

    uint8_t Shot(const Coords& coords);

    ~Kernel();
private:
    bool started_ = false;
    bool type_;  // тип игрока: 0 - slave; 1 - master
    Dimension dimension_;  // размерность поля
    Strategy* strategy_ = nullptr;  // выбранная стратегия
    Map* map_ = nullptr;  // наша карта
    std::array<uint64_t, 5> ships_;  // количество кораблей
    bool win_ = false;  // выиграна ли игра
};
