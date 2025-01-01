#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <boost/random.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <functional>
#include "map.h"

struct Generated {
/*
    Information of our generated info: dimension and ship counts.
*/
    Dimension dimension_;
    std::array<uint64_t, 5> ships_;
    Generated();
};

class Strategy {
/*
    Operate all our desision to win the enemy: make shots, set parameters (if we are master).
*/
public:
    Strategy(Dimension* dimension);
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    uint64_t GetShipsSum() const;
    virtual Coords Shot() = 0;
    virtual Generated Generate() = 0;
    virtual void KillShot();
    virtual void HitShot();
    virtual void MissShot() {}
    virtual ~Strategy() = default;
protected:
    Dimension* dimension_ = nullptr;
    uint64_t ships_sum_;
};

class OrderedStrategy : public Strategy {
public:
    OrderedStrategy(Dimension* d): Strategy(d) {}
    Coords Shot() override;
    Generated Generate() override;
private:
    Coords coords_ = {0, 0};
};

class CustomStrategy : public Strategy {
public:
    CustomStrategy(Dimension* d) : Strategy(d) {}
    Coords Shot() override;
    Generated Generate() override;
    void KillShot() override;
    void HitShot() override;
    void MissShot() override;
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    using UnorderedMap = boost::unordered_flat_map<Coords, Coords, CoordsHash>;
    UnorderedSet restricted_area_;
    Coords last_coords_ = {0, 0};
    UnorderedSet ship_destruction_;
    uint8_t strikes_;
    Coords rush_shot_();
    Coords search_shot_();
    bool is_rushing_ = false;
    const UnorderedMap moves_ = {
        {{0, 0}, {1, 1}}, // 1 -> 2
        {{1, 0}, {3, 0}}, // 12 -> 13
        {{2, 0}, {0, 2}}, // 6 -> 7
        {{3, 0}, {0, 1}}, // 13 -> 14
        {{0, 1}, {0, 3}}, // 14 -> 15
        {{1, 1}, {2, 2}}, // 2 -> 3
        {{2, 1}, {3, 1}}, // 8 -> 9
        {{3, 1}, {1, 2}}, // 9 -> 10
        {{0, 2}, {2, 1}}, // 7 -> 8
        {{1, 2}, {1, 3}}, // 10 -> 11
        {{2, 2}, {3, 2}}, // 3 -> 4
        {{3, 2}, {2, 3}}, // 4 -> 5
        {{0, 3}, {3, 3}}, // 15 -> 16
        {{1, 3}, {1, 0}}, // 11 -> 12
        {{2, 3}, {2, 0}}, // 5 -> 6
    };
};

class ExpStrategy : public Strategy {
public:
    ExpStrategy(Dimension* d) : Strategy(d) {}
    Coords Shot() override;
    Generated Generate() override;
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    UnorderedSet restricted_area_;
};

