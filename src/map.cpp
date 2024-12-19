#include "include/map.h"

bool Dimension::Empty() {
    return width_ == 0 || height_ == 0;
}

Map::Map(std::array<uint64_t, 4> ships, Dimension* dimension) {
    dimension_ = dimension;

    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));
    boost::random::uniform_int_distribution<> distribution(1, 4);
    int choosen_corner = distribution(generator);

    UnorderedMap restricted_area;  // храним запретные клетки
    UnorderedMap drawing;

    drawing_(0, 0, drawing, restricted_area);  // начальная прорисовка карты
    /// алгоритм вставки кораблей
    set_ship_(1, drawing, restricted_area);
}

void Map::drawing_(uint64_t x, uint64_t y, UnorderedMap& drawing, const UnorderedMap& restricted_area) {
    uint64_t right_wall = std::min(dimension_->width_, x + 4);
    uint64_t bottom_wall = std::min(dimension_->height_, y + 4);

    for (uint64_t i = 0; i < bottom_wall; ++i) {
        for (uint64_t j = 0; j < right_wall; ++j) {
            drawing[{x + j, y + i}] = get_rate_(x + j, y + i, restricted_area);
        }
    }
}

uint8_t Map::get_rate_(uint64_t x_pixel, uint64_t y_pixel, const UnorderedMap& restricted_area) const {
    uint8_t rate = 0;
    bool isTouchingTop = y_pixel == 0;
    bool isTouchingRight = x_pixel == dimension_->width_ - 1;
    bool isTouchingBottom = y_pixel == dimension_->height_ - 1;
    bool isTouchingLeft = x_pixel == 0;
    for (int8_t y = -1; y != 2; ++y) {
        for (int8_t x = -1; x != 2; ++x) {
            if (x == 0 && y == 0) continue;
            if ((isTouchingTop && y == -1) || (isTouchingRight && x == 1) || (isTouchingBottom && y == 1) || (isTouchingLeft && x == -1) ||
            restricted_area.find({x + x_pixel, y + y_pixel}) != restricted_area.end()) {
                ++rate;
            }
        }
    }
    return rate;
}

void Map::set_ship_(uint8_t type, UnorderedMap& drawing, UnorderedMap& restricted_area) {
    using Coords = std::pair<uint64_t, uint64_t>;
    using ShipPlacement = std::pair<uint8_t, bool>;  // false - horizontal, true - vertical
    using FuncRes = std::pair<ShipPlacement, bool>;

    std::vector<std::pair<Coords, ShipPlacement>> maxs;

    std::function<FuncRes(const Coords&)> can_be_horizontal = [&](const Coords& coords) {
        uint8_t rates = 0;
        for (uint i = 0; i != type; ++i) {
            if (auto iter = drawing.find({coords.first + i, coords.second}); iter == drawing.end()) return FuncRes(ShipPlacement(0, 0), true);
            else rates += iter->second;
        }
        return FuncRes(ShipPlacement(rates, 0), true);
    };
    std::function<FuncRes(const Coords&)> can_be_vertical = [&](const Coords& coords) {
        uint8_t rates = 0;
        for (uint i = 0; i != type; ++i) {
            if (auto iter = drawing.find({coords.first, coords.second + i}); iter == drawing.end()) return FuncRes(ShipPlacement(0, 1), false);
            else rates += iter->second;
        }
        return FuncRes(ShipPlacement(rates, 1), true);
    };
    std::function<void(const Coords&, const ShipPlacement&)> update_maxs = [&](const Coords& coords, const ShipPlacement& ship_placement) {
        if (maxs.empty()) {
            maxs.push_back({coords, ship_placement});
        } else {
            if (maxs[0].second.first <= ship_placement.first) {
                if (maxs[0].second.first < ship_placement.first) maxs.clear();
                maxs.push_back({coords, ship_placement});
            }
        }
    };

    for (const auto& [coords, rate] : drawing) {
        if (FuncRes finded = can_be_horizontal(coords); finded.second) {
            update_maxs(coords, finded.first);
        }
        if (FuncRes finded = can_be_vertical(coords); finded.second) {
            update_maxs(coords, finded.first);
        }
    }

    for (const auto& el : maxs) {
        std::cout << el.first.first << ' ' << el.first.second << " >> " << (int)el.second.first << (el.second.second ? " vertical" : " horizontal") << '\n';
    }
}
