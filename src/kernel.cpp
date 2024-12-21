#include "include/kernel.h"

Kernel::Kernel(bool type) : type_(type) {
    ships_.fill(0);
}

bool Kernel::SetWidth(uint64_t width) {
    dimension_.width_ = width;
    return true;
}
bool Kernel::SetHeight(uint64_t height) {
    dimension_.height_ = height;
    return true;
}
bool Kernel::SetCount(uint8_t type, uint64_t count) {
    if (type > 0 && type > 4) {
        return false;
    }
    ships_[type] = count;
    return true;
}
bool Kernel::SetStrategy(uint8_t type) {
    return strategy_.SetType(type);
}

uint64_t Kernel::GetWidth() const {
    return dimension_.width_;
}
uint64_t Kernel::GetHeight() const {
    return dimension_.height_;
}
uint64_t Kernel::GetCount(uint8_t type) const {
    if (type > 0 && type > 4) {
        return 0;
    }
    return ships_[type];
}

const Map& Kernel::GetMap() const {
    return *map_;
}
const Map& Kernel::GetEnemyMap() const {
    return *enemy_;
}

bool Kernel::IsReady() const {
    if (dimension_.Empty() || std::all_of(ships_.begin(), ships_.end(), [](const auto& x) {return x == 0;})) {
        return false;
    }
    return true;
}
bool Kernel::IsStarted() const {
    return started_;
}

bool Kernel::IsLoose() const {
    return map_->GetSize() == 0;
}

void Kernel::Start() {
    map_ = new Map(ships_, &dimension_);
    enemy_ = new Map(&dimension_);
    started_ = true;
    std::cout << *map_;
}

uint8_t Kernel::Shot(const Coords& coords) {
    return map_->Shot(coords);
}

Kernel::~Kernel() {
    if (map_ != nullptr) {
        delete map_;
    }
    if (enemy_ != nullptr) {
        delete enemy_;
    }
}
