#include "include/map.h"

bool Dimension::Empty() const {
    return width_ == 0 || height_ == 0;
}

bool Coords::operator == (const Coords& coords) const {
    return x == coords.x && y == coords.y;
}
bool Coords::operator < (const Coords& coords) const {
    return std::tie(x, y) < std::tie(coords.x, coords.y);
}
bool Coords::IsTouchingTop() const {
    return y == 0;
}
bool Coords::IsTouchingRight(const Dimension& dimension) const {
    return x == dimension.width_ - 1;
}
bool Coords::IsTouchingBottom(const Dimension& dimension) const {
    return y == dimension.height_ - 1;
}
bool Coords::IsTouchingLeft() const {
    return x == 0;
}

size_t CoordsHash::operator () (const Coords& coords) const {
    size_t seed = 0;
    boost::hash_combine(seed, coords.x);
    boost::hash_combine(seed, coords.y);
    return seed;
}

Map::Map(Dimension* dimension, std::array<uint64_t, 5> ships, uint64_t ships_sum) {
    dimension_ = dimension;
    ships_sum_ = ships_sum;

    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));

    UnorderedSet restricted_area;  // храним запретные клетки
    UnorderedMap drawing;  // прорисовка

    uint8_t min_type = std::find_if(ships.begin(), ships.end(), [](uint64_t ships_count) {
        return ships_count > 0;
    }) - ships.begin();

    draw_(drawing, restricted_area, {0, 0});

    for (uint8_t type = 4; type != 0; --type) {
        for (uint64_t i = 0; i != ships[type]; ++i) {
            if (uint64_t available_size = dimension_->width_ * dimension_->height_ - restricted_area.size();
            type == min_type && available_size >= (ships[type] - i) * type * type_k_[type]) {
                set_ship_(drawing, restricted_area, type, &generator);
            } else {
                set_ship_(drawing, restricted_area, type);
            }
        }
    }

    boost::random::uniform_int_distribution<> distribution(0, 3);
    int choosen_corner = distribution(generator);

    switch (choosen_corner) {
        case 0:
            map_ = flip_horizontal_();
            break;
        case 1:
            map_ = flip_vertical_();
            break;
        case 2:
            map_ = flip_diagonal_();
    }
}

uint8_t Map::HitShip(const Coords& coords) {
    auto iter = map_.find(coords);
    if (iter == map_.end()) return 0;
    map_.erase(iter);
    --ships_sum_;
    if ((!coords.IsTouchingTop() && map_.find({coords.x, coords.y - 1}) != map_.end())
    || (!coords.IsTouchingRight(*dimension_) && map_.find({coords.x + 1, coords.y}) != map_.end())
    || (!coords.IsTouchingBottom(*dimension_) && map_.find({coords.x, coords.y + 1}) != map_.end())
    || (!coords.IsTouchingLeft() && map_.find({coords.x - 1, coords.y}) != map_.end())) {
        return 1;
    }
    return 2;
}

size_t Map::GetSize() const {
    return map_.size();
}

const UnorderedSet& Map::GetMap() const {
    return map_;
}

void Map::SetShipForced(const Coords& coords, uint8_t type, bool rotate) {
    for (uint8_t i = 0; i != type; ++i) {
        map_.insert(rotate ? Coords{coords.x, coords.y + i} : Coords{coords.x + i, coords.y});
    }
}

void Map::draw_(UnorderedMap& drawing, const UnorderedSet& restricted_area, const Coords& coords) const {
    uint64_t right_wall = std::min(dimension_->width_, coords.x + 4);
    uint64_t bottom_wall = std::min(dimension_->height_, coords.y + 4);

    for (uint64_t y = coords.y; y < bottom_wall; ++y) {
        for (uint64_t x = coords.x; x < right_wall; ++x) {
            if (restricted_area.find({x, y}) == restricted_area.end()) {
                drawing[{x, y}] = get_rate_(restricted_area, {x, y});
            }
        }
    }
}

void Map::update_drawing_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate) const {
    uint8_t width = 1 + (type - 1) * !rotate;
    uint8_t height = 1 + (type - 1) * rotate;

    uint64_t bottom_line = coords.y + height + 1;
    uint64_t right_line = coords.x + width + 1;

    std::function<bool(uint64_t, uint8_t)> atX = [&](const uint64_t& x, uint8_t delta) {
        if (bottom_line >= dimension_->height_ || (coords.IsTouchingLeft() && delta < 0) || (x + delta >= dimension_->width_)) {
            return false;
        }
        return true;
    };
    std::function<bool(uint64_t, uint8_t)> atY = [&](const uint64_t& y, uint8_t delta) {
        if (right_line >= dimension_->width_ || (coords.IsTouchingTop() && delta < 0) || (y + delta >= dimension_->height_)) {
            return false;
        }
        return true;
    };

    for (int8_t x = -1; x < width + 1; x += 3) {
        if (!atX(coords.x, x)) {
            x -= 2;  // x' = (x + 3) - 3 + 1 = (x + 3) - 2
            continue;
        }
        draw_(drawing, restricted_area, {coords.x + x, bottom_line});
    }

    for (int8_t y = -1; y < height + 2; y += 3) {
        if (!atY(coords.y, y)) {
            y -= 2;  // y' = (y + 3) - 3 + 1 = (y + 3) - 2
            continue;
        }
        draw_(drawing, restricted_area, {right_line, coords.y + y});
    }
}

uint8_t Map::get_rate_(const UnorderedSet& restricted_area, const Coords& coords) const {
    uint8_t rate = 0;
    for (int8_t y = -1; y != 2; ++y) {
        for (int8_t x = -1; x != 2; ++x) {
            if (x == 0 && y == 0) continue;
            if ((coords.IsTouchingTop() && y == -1) 
            || (coords.IsTouchingRight(*dimension_) && x == 1) 
            || (coords.IsTouchingBottom(*dimension_) && y == 1) 
            || (coords.IsTouchingLeft() && x == -1) 
            || restricted_area.find({x + coords.x, y + coords.y}) != restricted_area.end()) {
                ++rate;
            }
        }
    }
    return rate;
}

void Map::choose_pixels_(const UnorderedMap& drawing, std::vector<std::pair<Coords, ShipSetting>>& maxs, uint8_t type) const {
    using FuncRes = std::pair<ShipSetting, bool>;

    std::function<FuncRes(const Coords&)> can_be_horizontal = [&](const Coords& coords) {
        uint8_t rates = 0;
        for (uint i = 0; i != type; ++i) {
            if (auto iter = drawing.find({coords.x + i, coords.y}); iter == drawing.end()) return FuncRes({0, 0}, false);
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

void Map::update_restricted_area_(UnorderedMap& drawing, UnorderedSet& restricted_area, const Coords& coords, uint8_t type, bool rotate) const {
    uint8_t width = 1 + (type - 1) * !rotate;
    uint8_t height = 1 + (type - 1) * rotate;
    Coords left_top = coords;
    Coords right_bottom = {coords.x + width - 1, coords.y + height - 1};

    for (int8_t y = -1; y != height + 1; ++y) {
        for (int8_t x = -1; x != width + 1; ++x) {
            if ((left_top.IsTouchingTop() && y == -1)
            || (right_bottom.IsTouchingRight(*dimension_) && x == width)
            || (right_bottom.IsTouchingBottom(*dimension_) && y == height)
            || (left_top.IsTouchingLeft() && x == -1)) {
                continue;
            }
            if (auto iter = drawing.find({coords.x + x, coords.y + y}); iter != drawing.end()) {
                drawing.erase(iter);
            }
            restricted_area.insert({coords.x + x, coords.y + y});
        }
    }
}

void Map::set_ship_(UnorderedMap& drawing, UnorderedSet& restricted_area, uint8_t type, boost::random::mt19937* randomizer) {

    if (randomizer != nullptr) {
        boost::random::uniform_int_distribution<> distributionX(0, dimension_->width_ - 1);
        boost::random::uniform_int_distribution<> distributionY(0, dimension_->height_ - 1);
        Coords coords = {static_cast<uint64_t>(distributionX(*randomizer)), static_cast<uint64_t>(distributionY(*randomizer))};

        std::function<bool(const Coords&)> can_be_horizontal = [&](const Coords& coords) {
            for (uint i = 0; i != type; ++i) {
                if (auto iter = restricted_area.find({coords.x + i, coords.y}); iter != restricted_area.end() || coords.x + i >= dimension_->width_) return false;
            }
            return true;
        };
        std::function<bool(const Coords&)> can_be_vertical = [&](const Coords& coords) {
            for (uint i = 0; i != type; ++i) {
                if (auto iter = restricted_area.find({coords.x, coords.y + i}); iter != restricted_area.end() || coords.y + i >= dimension_->height_) return false;
            }
            return true;
        };

        bool h = can_be_horizontal(coords);
        bool v = can_be_vertical(coords);
        if (h || v) {
            update_restricted_area_(drawing, restricted_area, coords, type, h ? false : true);
            for (uint8_t i = 0; i != type; ++i) {
                if (h) {  // vertical
                    map_.insert({coords.x + i, coords.y});
                } else {  // horizontal
                    map_.insert({coords.x, coords.y + i});
                }
            }
            return;
        }
    }

    std::vector<std::pair<Coords, ShipSetting>> maxs;

    choose_pixels_(drawing, maxs, type);

    std::sort(maxs.begin(), maxs.end(), [](const std::pair<Coords, ShipSetting>& lhs_, const std::pair<Coords, ShipSetting>& rhs_) {
        return pow(lhs_.first.x, 2) + pow(lhs_.first.y, 2) < pow(rhs_.first.x, 2) + pow(rhs_.first.y, 2);
    });

    if (maxs.empty()) return;  // safely out if we have no place.

    std::pair<Coords, ShipSetting>& choosen = maxs[0];
    const Coords& coords = choosen.first;
    const auto& x = coords.x;
    const auto& y = coords.y;
    for (uint8_t i = 0; i != type; ++i) {
        if (choosen.second.rotate) {  // vertical
            map_.insert({x, y + i});
        } else {  // horizontal
            map_.insert({x + i, y});
        }
    }

    update_restricted_area_(drawing, restricted_area, coords, type, choosen.second.rotate);

    update_drawing_(drawing, restricted_area, coords, type, choosen.second.rotate);
}

UnorderedSet Map::flip_horizontal_() {
    UnorderedSet flipped;
    for (const Coords& coords : map_) {
        flipped.insert({dimension_->width_ - coords.x - 1, coords.y});
    }
    return flipped;
}
UnorderedSet Map::flip_vertical_() {
    UnorderedSet flipped;
    for (const Coords& coords : map_) {
        flipped.insert({coords.x, dimension_->height_ - coords.y - 1});
    }
    return flipped;
}
UnorderedSet Map::flip_diagonal_() {
    UnorderedSet flipped;
    for (const Coords& coords : map_) {
        flipped.insert({dimension_->width_ - coords.x - 1, dimension_->height_ - coords.y - 1});
    }
    return flipped;
}

std::ostream& operator << (std::ostream& out, const Map& map) {
    std::vector<Coords> buffer(map.map_.begin(), map.map_.end());  // copy pair, cause can implemet more effect out
    std::sort(buffer.begin(), buffer.end(), [](const Coords& lhs, const Coords& rhs) {
        return std::tie(lhs.y, lhs.x) > std::tie(rhs.y, rhs.x);
    });
    for (size_t y = 0; y != map.dimension_->height_; ++y) {
        for (size_t x = 0; x != map.dimension_->width_; ++x) {
            if (!buffer.empty() && x == buffer.rbegin()->x && y == buffer.rbegin()->y) {
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
