#include "include/map.h"

bool Dimension::Empty() {
    return width_ == 0 || height_ == 0;
}

bool Coords::operator == (const Coords& coords) const {
    return x == coords.x && y == coords.y;
}

std::size_t Map::CoordsHash::operator () (const Coords& coords) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, coords.x);
    boost::hash_combine(seed, coords.y);
    return seed;
}

Map::Map(std::array<uint64_t, 5> ships, Dimension* dimension) {
    dimension_ = dimension;

    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));
    boost::random::uniform_int_distribution<> distribution(1, 4);
    int choosen_corner = distribution(generator);

    UnorderedSet restricted_area;  // храним запретные клетки
    UnorderedMap drawing;  // прорисовка

    drawing_(drawing, restricted_area, 0, 0);  // начальная прорисовка карты
    /// алгоритм вставки кораблей
    for (uint8_t type = 4; type != 0; --type) {
        for (uint64_t i = 0; i != ships[type]; ++i) {
            set_ship_(drawing, restricted_area, type);
            // здесь нужно заново прорисовывать карту)
        }
    }
}

void Map::drawing_(UnorderedMap& drawing, const UnorderedSet& restricted_area, uint64_t x, uint64_t y) {
    uint64_t right_wall = std::min(dimension_->width_, x + 4);
    uint64_t bottom_wall = std::min(dimension_->height_, y + 4);

    for (uint64_t i = 0; i < bottom_wall; ++i) {
        for (uint64_t j = 0; j < right_wall; ++j) {
            drawing[{x + j, y + i}] = get_rate_(restricted_area, x + j, y + i);
        }
    }
}

uint8_t Map::get_rate_(const UnorderedSet& restricted_area, uint64_t x_pixel, uint64_t y_pixel) const {
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

void Map::choose_pixels_(const UnorderedMap& drawing, std::vector<std::pair<Coords, ShipSetting>>& maxs, uint8_t type) {
    using FuncRes = std::pair<ShipSetting, bool>;

    std::function<FuncRes(const Coords&)> can_be_horizontal = [&](const Coords& coords) {
        uint8_t rates = 0;
        for (uint i = 0; i != type; ++i) {
            if (auto iter = drawing.find({coords.x + i, coords.y}); iter == drawing.end()) return FuncRes({0, 0}, true);
            else rates += iter->second;
        }
        return FuncRes({rates, 0}, true);
    };
    std::function<FuncRes(const Coords&)> can_be_vertical = [&](const Coords& coords) {
        uint8_t rates = 0;
        for (uint i = 0; i != type; ++i) {
            if (auto iter = drawing.find({coords.x, coords.y + i}); iter == drawing.end()) return FuncRes({0, 1}, false);
            else rates += iter->second;
        }
        return FuncRes({rates, 1}, true);
    };
    std::function<void(const Coords&, const ShipSetting&)> update_maxs = [&](const Coords& coords, const ShipSetting& ship_setting) {
        if (maxs.empty()) {
            maxs.push_back({coords, ship_setting});
        } else {
            if (maxs[0].second.rate <= ship_setting.rate) {
                if (maxs[0].second.rate < ship_setting.rate) maxs.clear();
                maxs.push_back({coords, ship_setting});
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
}

void Map::update_restricted_area_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate) {
    bool isTouchingTop = coords.y == 0;
    bool isTouchingRight = coords.x == dimension_->width_ - 1;
    bool isTouchingBottom = coords.y == dimension_->height_ - 1;
    bool isTouchingLeft = coords.x == 0;
    for (int8_t y = -1; y != 2; ++y) {
        for (int8_t x = -1; x != type + 1; ++x) {
            if ((isTouchingTop && y == -1) || (isTouchingRight && x == dimension_->width_) || (isTouchingBottom && y == 1) || (isTouchingLeft && x == -1)) {
                continue;
            }
            if (auto iter = drawing.find({coords.x + x, coords.y + y}); iter != drawing.end()) {
                drawing.erase(iter);
            }
            restricted_area.insert({coords.x + x, coords.y + y});
        }
    }

    //  clearing restricted area
    for (auto iter = restricted_area.begin(); iter != restricted_area.end();) {
        if (get_rate_(restricted_area, iter->x, iter->y) == 8) {
            iter = restricted_area.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Map::set_ship_(UnorderedMap& drawing, UnorderedSet& restricted_area, uint8_t type) {
    std::vector<std::pair<Coords, ShipSetting>> maxs;

    choose_pixels_(drawing, maxs, type);

    std::sort(maxs.begin(), maxs.end(), [](const std::pair<Coords, ShipSetting>& lhs_, const std::pair<Coords, ShipSetting>& rhs_) {
        return pow(lhs_.first.x, 2) + pow(lhs_.first.y, 2) < pow(rhs_.first.x, 2) + pow(rhs_.first.y, 2);
    });

    if (maxs.empty()) return;  // safely out if we have no place.

    std::pair<Coords, ShipSetting>& choosen = maxs[0];
    // std::cout << "CHOOSEN CORDS " << choosen.first.x << ' ' << choosen.first.y << '\n';
    const auto& x = choosen.first.x;
    const auto& y = choosen.first.y;
    for (uint8_t i = 0; i != type; ++i) {
        if (choosen.second.rotate) {  // vertical
            map_[{x, y + i}] = 1;
        } else {  // horizontal
            map_[{x + i, y}] = 1;
        }
    }

    update_restricted_area_(drawing, restricted_area, choosen.first, type, choosen.second.rotate);

    // std::cout << "look at the drawing\n";
    // for (const auto& [key, value] : drawing) {
    //     std::cout << key.x << ' ' << key.y << '\n';
    // }
    // std::cout << '\n';
}

std::ostream& operator << (std::ostream& out, const Map& map) {
    std::vector<std::pair<Coords, uint8_t>> buffer(map.map_.begin(), map.map_.end());  // copy pair, cause can implemet more effect out
    std::sort(buffer.begin(), buffer.end(), [](const std::pair<Coords, uint8_t>& lhs, const std::pair<Coords, uint8_t>& rhs) {
        return std::tie(lhs.first.y, lhs.first.x) > std::tie(rhs.first.y, rhs.first.x);
    });
    for (size_t y = 0; y != map.dimension_->height_; ++y) {
        for (size_t x = 0; x != map.dimension_->width_; ++x) {
            if (!buffer.empty() && x == buffer.rbegin()->first.x && y == buffer.rbegin()->first.y) {
                out << "⏚ ";
                buffer.erase(std::prev(buffer.end()));
            } else {
                out << "⏝ ";
            }
        }
        out << "\n";
    }
    return out;
}


// create master
// set width 6
// set height 6
// set count 4 1
// set count 2 2
// start