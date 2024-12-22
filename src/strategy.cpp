#include "include/strategy.h"

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
void Strategy::SetShot() {
    if (ships_sum_ > 0) {
        --ships_sum_;
    }
}

Coords OrderedStrategy::Shot() {
    Coords coords_out = coords_;
    coords_.x = (coords_.x + 1) % GetWidth();
    coords_.y = (coords_.y + 1 * (coords_.x == 0)) % GetHeight();
    return coords_out;
}

Coords CustomStrategy::Shot() {
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
