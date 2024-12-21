#include "include/strategy.h"

void Strategy::Set(Dimension* dimension, const std::array<uint64_t, 5>& ships) {
    dimension_ = dimension;
    ships_sum_ = std::accumulate(ships.begin(), ships.end(), 0, std::plus<uint64_t>());
}
uint64_t Strategy::GetWidth() const {
    return dimension_->width_;
}
uint64_t Strategy::GetHeight() const {
    return dimension_->height_;
}
void Strategy::HitShip() {
    if (ships_sum_ > 0)
        --ships_sum_;
}

Coords OrderedStrategy::Next() {
    Coords coords_out = coords_;
    coords_.x = (coords_.x + 1) % GetWidth();
    coords_.y = (coords_.y + 1 * (coords_.x == 0)) % GetHeight();
    return coords_out;
}

Coords CustomStrategy::Next() {
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
    const Coords& choosen_coords = avaible_coords[distribution(generator)];
    restricted_area_.insert(choosen_coords);
    return choosen_coords;
}
