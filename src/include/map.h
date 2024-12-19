#pragma once
#include <array>
#include <algorithm>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/random.hpp>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>

struct Dimension {
    uint64_t width_;
    uint64_t height_;
    bool Empty();
};

class Map {
/*
    Warning: the given value to the constructor must be correct!
*/
public:
    Map() = default;
    Map(std::array<uint64_t, 4> ships, Dimension* dimension);
private:
    using UnorderedMap = boost::unordered_flat_map<std::pair<uint64_t, uint64_t>, uint8_t, boost::hash<std::pair<uint64_t, uint64_t>>>;
    UnorderedMap map_;
    Dimension* dimension_ = nullptr;

    void drawing_(uint64_t x, uint64_t y, UnorderedMap& drawing, const UnorderedMap& restricted_area);
    uint8_t get_rate_(uint64_t x, uint64_t y, const UnorderedMap& restricred_area) const;
    void set_ship_(uint8_t type, UnorderedMap& drawing, UnorderedMap& restricted_area);
};
