#pragma once
#include <array>
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <tuple>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/random.hpp>

struct Dimension {
    uint64_t width_;
    uint64_t height_;
    bool Empty() const;
};

struct Coords {
    uint64_t x;
    uint64_t y;
    Coords(uint64_t x_, uint64_t y_) : x(x_), y(y_) {}
    bool operator == (const Coords& coords) const;
    bool operator < (const Coords& coords) const;
    bool IsTouchingTop() const ;
    bool IsTouchingRight(const Dimension& dimension) const ;
    bool IsTouchingBottom(const Dimension& dimension) const;
    bool IsTouchingLeft() const;
};

struct CoordsHash {
    std::size_t operator () (const Coords& coords) const;
};

using UnorderedMap = boost::unordered_flat_map<Coords, uint8_t, CoordsHash>;
using UnorderedSet = boost::unordered_set<Coords, CoordsHash>;

class Map {
/*
    Map: creating and operating shifts / pixels.
*/
public:
    Map(Dimension* dimension, std::array<uint64_t, 5> ships, uint64_t ships_sum);
    uint8_t HitShip(const Coords& coords);  // 0 - miss, 1 - hit, 2 - kill
    size_t GetSize() const;
    const UnorderedSet& GetMap() const;
    void SetShipForced(const Coords& coords, uint8_t type, bool rotate);

    friend std::ostream& operator << (std::ostream& out, const Map& map);
private:
    struct ShipSetting {
        uint8_t rate;
        bool rotate;  // false - horizontal, true - vertical
    };

    UnorderedSet map_;
    uint64_t ships_sum_;
    Dimension* dimension_ = nullptr;
    std::map<uint8_t, uint8_t> type_k_ = {  // koefficient for ship's type (ship area + restricted adds)
        {1, 9},
        {2, 6},
        {3, 5},
        {4, 5}
    };

    void draw_(UnorderedMap& drawing, const UnorderedSet& restricted_area, const Coords& coords) const;
    void update_drawing_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate) const;
    uint8_t get_rate_(const UnorderedSet& restricted_area, const Coords& coords) const;
    void choose_pixels_(const UnorderedMap& drawing, std::vector<std::pair<Coords, ShipSetting>>& maxs, uint8_t type) const;
    void update_restricted_area_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate) const;
    void set_ship_(UnorderedMap& drawing, UnorderedSet& restricted_area, uint8_t type, boost::random::mt19937* randomizer = nullptr);
};
