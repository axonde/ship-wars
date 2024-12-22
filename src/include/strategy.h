#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <boost/random.hpp>
#include <boost/unordered_set.hpp>
#include "map.h"

class Strategy {
/*
    Operate all our desision to win the enemy: make shots, set parameters (if we are master).
*/
public:
    Strategy(Dimension* dimension);
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    virtual Coords Shot() = 0;
    void SetShot();
    virtual ~Strategy() = default;
private:
    uint64_t ships_sum_;
    Dimension* dimension_ = nullptr;
};

class OrderedStrategy : public Strategy {
    using Strategy::Strategy;
public:
    Coords Shot() override;
private:
    Coords coords_ = {0, 0};
};

class CustomStrategy : public Strategy {
    using Strategy::Strategy;
public:
    Coords Shot() override;
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    UnorderedSet restricted_area_;
};
