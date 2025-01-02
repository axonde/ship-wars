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
void Strategy::SetKill() {
    SetHit();
}
void Strategy::SetHit() {
    if (ships_sum_ > 0) {
        --ships_sum_;
    }
}

/* ORDERED */
Coords OrderedStrategy::Shot() {
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
/*
    If all elements are mark out -> UB. (have an undefined while)
*/
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
Coords CustomStrategy::Shot() {
    is_rushing_ ? rush_() : search_();
    return candidate_;
}
void CustomStrategy::SetKill() {
    // is_rushing_ = false;
    // if (ships_sum_ > 0) {
    //     --ships_sum_;
    // }
}
void CustomStrategy::SetHit() {
    is_rushing_ = true;
    target_.insert(candidate_);
    set_candidates_();
}
void CustomStrategy::SetMiss() {
//     std::cout << "setting miss\n";
//     if (is_rushing_) {
//         if (auto pixel = ship_destruction_.find(last_coords_); pixel != ship_destruction_.end()) {
//             ship_destruction_.erase(pixel);
//         }
//     }
//     if (ship_destruction_.size() == 0) {
//         is_rushing_ = false;
//     }
// }
// Coords CustomStrategy::rush_shot_() {
//     last_coords_ = *ship_destruction_.begin();
//     ship_destruction_.erase(ship_destruction_.begin());
//     restricted_area_.insert(last_coords_);
//     return last_coords_;
}
void CustomStrategy::set_candidates_() {
    std::function<void(bool)> Clear = [&](bool rotated) {
        for (auto pixel = candidates_.begin(); pixel != candidates_.end(); ++pixel) {
            if (rotated) {
                if (pixel->x != candidate_.x) {
                    pixel = candidates_.erase(pixel);
                }
            } else {
                if (pixel->y != candidate_.y) {
                    pixel = candidates_.erase(pixel);
                }
            }
        }
    };

    if (target_.size() == 0) {
        if (!candidate_.IsTouchingLeft()) {
            candidates_.insert({candidate_.x - 1, candidate_.y});
        }
        if (!candidate_.IsTouchingTop()) {
            candidates_.insert({candidate_.x, candidate_.y - 1});
        }
        if (!candidate_.IsTouchingRight(*dimension_)) {
            candidates_.insert({candidate_.x + 1, candidate_.y});
        }
        if (!candidate_.IsTouchingBottom(*dimension_)) {
            candidates_.insert({candidate_.x, candidate_.y + 1});
        }
        return;
    }
    Coords cxl = {candidate_.x - 1, candidate_.y};  // coord x left
    Coords cxr = {candidate_.x + 1, candidate_.y};  // coord x right
    Coords cyb = {candidate_.x, candidate_.y + 1};  // coord y bottom
    Coords cyt = {candidate_.x, candidate_.y - 1};  // coord y top
    if (target_.find(cxl) != target_.end() || target_.find(cxr) != target_.end()) {  // horizontal
        if (target_.find(cxl) == target_.end() && !candidate_.IsTouchingLeft() && restricted_area_.find(cxl) == restricted_area_.end()) {
            candidates_.insert(cxl);
        }
        if (target_.find(cxr) == target_.end() && !candidate_.IsTouchingRight(*dimension_) && restricted_area_.find(cxr) == restricted_area_.end()) {
            candidates_.insert(cxr);
        }
        Clear(0);
    } else {  // vertical
        if (target_.find(cyb) == target_.end() && !candidate_.IsTouchingBottom(*dimension_) && restricted_area_.find(cyb) == restricted_area_.end()) {
            candidates_.insert(cyb);
        }
        if (target_.find(cyt) == target_.end() && !candidate_.IsTouchingTop() && restricted_area_.find(cyt) == restricted_area_.end()) {
            candidates_.insert(cyt);
        }
        Clear(1);
    }
}
void CustomStrategy::rush_() {
    std::cout << "debug!! " << candidates_.size() << '\n';
    candidate_ = *candidates_.begin();
    restricted_area_.insert(candidate_);
    candidates_.erase(candidates_.begin());
}
void CustomStrategy::search_() {
    do {
        next_();
    } while (candidate_.x >= dimension_->width_ || candidate_.y >= dimension_->height_ || restricted_area_.find(candidate_) != restricted_area_.end());
}
void CustomStrategy::next_() {
    const uint64_t x = candidate_.x / 4 * 4;
    const uint64_t y = candidate_.y / 4 * 4;
    uint8_t x_mod = candidate_.x % 4;
    uint8_t y_mod = candidate_.y % 4;
    Coords move = moves_.find({x_mod, y_mod})->second;

    auto next = std::find(firing_[level_].begin(), firing_[level_].end(), move);
    restricted_area_.insert(candidate_);
    if (next != firing_[level_].end()) {
        candidate_ = {x + move.x, y + move.y};
        return;
    }
    next_square_();
    candidate_ = {candidate_.x + firing_[level_ % 4][0].x, candidate_.y + firing_[level_ % 4][0].y};
}
void CustomStrategy::next_square_() {
    uint64_t& x = candidate_.x;
    uint64_t& y = candidate_.y;
    const uint8_t x_mod = x % 4;
    const uint8_t y_mod = y % 4;
    if (x / 4 * 4 + 4 > dimension_->width_) {
        x = 0;
        if (y / 4 * 4 + 4 > dimension_->height_) {
            ++level_;
            y = 0;
        } else {
            y = y / 4 * 4 + 4;
        }
    } else {
        x = x / 4 * 4 + 4;
        y = y / 4 * 4;
    }
}

/* old custom strategy -> now obsolete */
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
