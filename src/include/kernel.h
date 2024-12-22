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
    void SetShot();
    void SetShipForced(const Coords& coords, uint8_t type, bool rotate);

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    uint64_t GetCount(uint8_t type) const;
    const Map& GetMap() const;
    const bool GetType() const;

    bool IsReady() const;
    bool IsStarted() const;
    bool IsStopped() const;
    bool IsWin() const;
    bool IsLose() const;
    bool IsStrategySet() const;

    void Start();
    void Stop();
    uint8_t HitShip(const Coords& coords);
    Coords Shot();

    ~Kernel();
private:
    bool started_ = false;
    bool stopped_ = false;
    bool type_;  // тип игрока: 0 - slave; 1 - master
    Dimension dimension_;  // размерность поля
    Strategy* strategy_ = nullptr;  // выбранная стратегия
    Map* map_ = nullptr;  // наша карта
    std::array<uint64_t, 5> ships_;  // количество кораблей
    bool win_ = false;  // выиграна ли игра
};
