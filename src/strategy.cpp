#include "include/strategy.h"

Generated::Generated() {
    ships_.fill(0);
}

Strategy::Strategy(Dimension* dimension) {
    dimension_ = dimension;
}
uint64_t Strategy::GetWidth() const {
    if (dimension_ != nullptr) {
        return dimension_->width_;
    }
    return 0;
}
uint64_t Strategy::GetHeight() const {
    if (dimension_ != nullptr) {
        return dimension_->height_;
    }
    return 0;
}
uint64_t Strategy::GetShipsSum() const {
    return ships_sum_;
}
void Strategy::HitShot() {
    KillShot();
}
void Strategy::KillShot() {
    if (ships_sum_ > 0) {
        --ships_sum_;
    }
}

/* ORDERED */
Coords OrderedStrategy::Shot() {
    /*
        If all elements are mark out -> UB. (have an undefined while)
    */
    Coords coords_out = coords_;
    coords_.x = (coords_.x + 1) % GetWidth();
    coords_.y = (coords_.y + 1 * (coords_.x == 0)) % GetHeight();
    return coords_out;
}
Generated OrderedStrategy::Generate() {
    Generated generated;
    generated.dimension_ = {4 * 30 + 29, 101};
    generated.ships_ = {0, 0, 0, 0, 51 * 30};
    return generated;
}

/* CUSTOM */
Coords CustomStrategy::Shot() {
    if (is_rushing_) {
        return rush_shot_();
    }
    return search_shot_();
}
void CustomStrategy::KillShot() {
    is_rushing_ = false;
}
void CustomStrategy::HitShot() {
    std::function<void(bool)> Clear = [&](bool rotated) {
        for (auto pixel = ship_destruction_.begin(); pixel != ship_destruction_.end(); ++pixel) {
            if (rotated) {
                if (pixel->x != last_coords_.x) {
                    pixel = ship_destruction_.erase(pixel);
                }
            } else {
                if (pixel->y != last_coords_.y) {
                    pixel = ship_destruction_.erase(pixel);
                }
            }
        }
    };

    if (!is_rushing_) {
        strikes_ = 0;
        is_rushing_ = true;
        ship_destruction_.clear();
        if (!last_coords_.IsTouchingLeft()) {
            ship_destruction_.insert({last_coords_.x - 1, last_coords_.y});
        } else if (!last_coords_.IsTouchingTop()) {
            ship_destruction_.insert({last_coords_.x, last_coords_.y - 1});
        }
        if (!last_coords_.IsTouchingRight(*dimension_)) {
            ship_destruction_.insert({last_coords_.x + 1, last_coords_.y});
        }
        if (!last_coords_.IsTouchingBottom(*dimension_)) {
            ship_destruction_.insert({last_coords_.x, last_coords_.y + 1});
        }
    } else {
        if (strikes_ < 4) {
            if (ship_destruction_.find({last_coords_.x - 1, last_coords_.y}) != ship_destruction_.end()
            || ship_destruction_.find({last_coords_.x + 1, last_coords_.y}) != ship_destruction_.end()) {  // horizontal
                if (restricted_area_.find({last_coords_.x - 1, last_coords_.y}) == restricted_area_.end()) {
                    ship_destruction_.insert({last_coords_.x - 1, last_coords_.y});
                }
                if (restricted_area_.find({last_coords_.x + 1, last_coords_.y}) == restricted_area_.end()) {
                    ship_destruction_.insert({last_coords_.x + 1, last_coords_.y});
                }
                Clear(0);
            } else {  // vertical
                if (restricted_area_.find({last_coords_.x, last_coords_.y - 1}) == restricted_area_.end()) {
                    ship_destruction_.insert({last_coords_.x, last_coords_.y - 1});
                }
                if (restricted_area_.find({last_coords_.x, last_coords_.y + 1}) == restricted_area_.end()) {
                    ship_destruction_.insert({last_coords_.x, last_coords_.y + 1});
                }
                Clear(1);
            }
            ++strikes_;
        }
    }
    if (ships_sum_ > 0) {
        --ships_sum_;
    }
}
void CustomStrategy::MissShot() {
    if (is_rushing_) {
        ship_destruction_.erase(ship_destruction_.find(last_coords_));
    }
    if (ship_destruction_.size() == 0) {
        is_rushing_ = false;
    }
}
Coords CustomStrategy::rush_shot_() {
    last_coords_ = *ship_destruction_.begin();
    ship_destruction_.erase(ship_destruction_.begin());
    restricted_area_.insert(last_coords_);
    return last_coords_;
}
Coords CustomStrategy::search_shot_() {
    std::function<void()> Next = [&]() {
        do {
            uint8_t x_mod = last_coords_.x % 4;
            uint8_t y_mod = last_coords_.y % 4;
            if (x_mod == 3 && y_mod == 3) {
                if (last_coords_.x + 1 < dimension_->width_) {
                    ++last_coords_.x;
                    last_coords_.y -= y_mod;
                } else {
                    last_coords_.x = 0;
                    ++last_coords_.y;
                }
                return;
            }
            last_coords_ = {(last_coords_.x / 4) * 4 + moves_.find({x_mod, y_mod})->second.x, (last_coords_.y / 4) * 4 + moves_.find({x_mod, y_mod})->second.y};
        } while (last_coords_.y >= dimension_->height_);
    };

    while (restricted_area_.find(last_coords_) != restricted_area_.end()) {
        Next();
    }
    restricted_area_.insert(last_coords_);
    return last_coords_;
}
Generated CustomStrategy::Generate() {
    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));
    boost::random::uniform_int_distribution<> distribution_size(15, 100);
    boost::random::uniform_int_distribution<> distribution_ships(2, 10);

    Generated generated;
    uint64_t size = distribution_size(generator);
    generated.dimension_ = {size, size};
    for (uint8_t i = 1; i != 5; ++i) {
        generated.ships_[i] = distribution_ships(generator);
    }
    return generated;
}

Coords ExpStrategy::Shot() {
    /*
        very bad choice if you have big dimension and a small quantity of avaible pixel...
        --> u will just iterate it all!! <--
    */
    const uint8_t max_next_coords_count = 40;
    std::vector<Coords> avaible_coords;
    for (uint64_t y = 0; y != GetHeight(); ++y) {
        for (uint64_t x = 0; x != GetWidth(); ++x) {
            if (restricted_area_.find({x, y}) != restricted_area_.end()) {
                continue;
            }
            avaible_coords.push_back(Coords{x, y});
            if (avaible_coords.size() > 40) break;
        }
        if (avaible_coords.size() > 40) break;
    }
    
    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));
    boost::random::uniform_int_distribution<> distribution(0, 39);
    if (avaible_coords.size() == 0) {
        return {0, 0};  // define the UB
    }
    const Coords& choosen_coords = avaible_coords[std::min(static_cast<size_t>(distribution(generator)), avaible_coords.size() - 1)];
    restricted_area_.insert(choosen_coords);
    return choosen_coords;
}
Generated ExpStrategy::Generate() {
    boost::random::mt19937 generator(static_cast<unsigned>(std::time(0)));
    boost::random::uniform_int_distribution<> distribution_size(15, 100);
    boost::random::uniform_int_distribution<> distribution_ships(2, 10);

    Generated generated;
    uint64_t size = distribution_size(generator);
    generated.dimension_ = {size, size};
    for (uint8_t i = 1; i != 5; ++i) {
        generated.ships_[i] = distribution_ships(generator);
    }
    return generated;
}
