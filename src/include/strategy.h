#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <functional>
#include <set>
#include <vector>
#include <boost/random.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
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
    Analyze of the enemy's game.
*/
public:
    Strategy(Dimension* d, uint64_t s) : dimension_(d), ships_sum_(s) {}
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    uint64_t GetShipsSum() const;
    void UpdateShipsSum(uint64_t ships_sum);
    virtual Coords Shot() = 0;
    virtual Generated Generate() = 0;
    virtual void SetKill();
    virtual void SetHit();
    virtual void SetMiss() {}
    virtual ~Strategy() = default;
protected:
    Dimension* dimension_ = nullptr;
    uint64_t ships_sum_;
};

class OrderedStrategy : public Strategy {
public:
    OrderedStrategy(Dimension* d, uint64_t s): Strategy(d, s) {}
    Coords Shot() override;
    Generated Generate() override;
private:
    Coords coords_ = {0, 0};
};

class CustomStrategy : public Strategy {
public:
    CustomStrategy(Dimension* d, uint64_t s) : Strategy(d, s) {}
    Coords Shot() override;
    Generated Generate() override;
    void SetKill() override;
    void SetHit() override;
    void SetMiss() override;
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    using UnorderedMap = boost::unordered_flat_map<Coords, Coords, CoordsHash>;
    UnorderedSet restricted_area_;
    Coords candidate_ = {0, 0};
    Coords rush_mark_ = {0, 0};
    UnorderedSet candidates_;
    std::set<Coords> target_;
    void rush_();
    void set_candidates_();
    void update_target_area_();
    void destroy_ship_();
    void search_();
    void next_();
    void next_square_();
    bool is_rushing_ = false;
    uint8_t level_ = 0;
    const std::array<std::vector<Coords>, 4> firing_ = {
        std::vector<Coords>{{0, 0}, {1, 1}, {3, 2}, {2, 3}},  // search battleships (линкоры)
        std::vector<Coords>{{2, 0}, {0, 2}},  // search cruisers (крейсеры)
        std::vector<Coords>{{2, 1}, {3, 1}, {1, 2}, {1, 3}},  // search destroyers (эсминцы)
        std::vector<Coords>{{1, 0}, {3, 0}, {0, 1}, {2, 2}, {0, 3}, {3, 3}}  // search torpedo boats (торпедные катеры)
    };
    const UnorderedMap moves_ = {
        {{0, 0}, {1, 1}}, // 1 -> 2
        {{1, 0}, {3, 0}}, // 11 -> 12
        {{2, 0}, {0, 2}}, // 5 -> 6
        {{3, 0}, {0, 1}}, // 12 -> 13
        {{0, 1}, {2, 2}}, // 13 -> 14
        {{1, 1}, {3, 2}}, // 2 -> 3
        {{2, 1}, {3, 1}}, // 7 -> 8
        {{3, 1}, {1, 2}}, // 8 -> 9
        {{0, 2}, {2, 1}}, // 6 -> 7
        {{1, 2}, {1, 3}}, // 9 -> 10
        {{2, 2}, {0, 3}}, // 14 -> 15
        {{3, 2}, {2, 3}}, // 3 -> 4
        {{0, 3}, {3, 3}}, // 15 -> 16
        {{1, 3}, {1, 0}}, // 10 -> 11
        {{2, 3}, {2, 0}}, // 4 -> 5
    };
};

class ExpStrategy : public Strategy {
public:
    ExpStrategy(Dimension* d, uint64_t s) : Strategy(d, s) {}
    Coords Shot() override;
    Generated Generate() override;
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    UnorderedSet restricted_area_;
};
