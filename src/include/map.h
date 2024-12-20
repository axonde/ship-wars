#pragma once
#include <array>
#include <algorithm>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/random.hpp>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <tuple>

struct Dimension {
    uint64_t width_;
    uint64_t height_;
    bool Empty();
};

struct Coords {
    uint64_t x;
    uint64_t y;
    bool operator == (const Coords& coords) const;
};

class Map {
/*
    Warning: the given value to the constructor must be correct!
*/
public:
    Map() = default;
    Map(std::array<uint64_t, 5> ships, Dimension* dimension);

    friend std::ostream& operator << (std::ostream& out, const Map& map);
private:
    struct CoordsHash {
        std::size_t operator () (const Coords& coords) const;
    };
    struct ShipSetting {
        uint8_t rate;
        bool rotate;  // false - horizontal, true - vertical
    };

    using UnorderedMap = boost::unordered_flat_map<Coords, uint8_t, CoordsHash>;
    using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;
    UnorderedMap map_;
    Dimension* dimension_ = nullptr;

    void drawing_(UnorderedMap& drawing, const UnorderedSet& restricted_area, uint64_t x, uint64_t y);
    uint8_t get_rate_(const UnorderedSet& restricted_area, uint64_t x, uint64_t y) const;
    void choose_pixels_(const UnorderedMap& drawing, std::vector<std::pair<Coords, ShipSetting>>& maxs, uint8_t type);
    void update_restricted_area_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate);
    void set_ship_(UnorderedMap& drawing, UnorderedSet& restricted_area, uint8_t type);
};
