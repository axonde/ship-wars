#pragma once
#include <array>
#include <cstdint>
#include <boost/random.hpp>
#include <boost/unordered_set.hpp>
#include <numeric>
#include "map.h"

class Strategy {
public:
    void Set(Dimension* dimension, const std::array<uint64_t, 5>& ships);
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    void HitShip();
    virtual Coords Next() = 0;
private:
    uint64_t ships_sum_;
    Dimension* dimension_ = nullptr;
};

class OrderedStrategy : public Strategy {
public:
    Coords Next();
private:
    Coords coords_ = {0, 0};
};

class CustomStrategy : public Strategy {
public:
    Coords Next();
private:
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    UnorderedSet restricted_area_;
};
