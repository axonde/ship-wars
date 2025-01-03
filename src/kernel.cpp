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
void Kernel::SetOrderedStrategy() {
    strategy_ = new OrderedStrategy(&dimension_, ships_sum_);
}
void Kernel::SetCustomStrategy() {
    strategy_ = new CustomStrategy(&dimension_, ships_sum_);
}
void Kernel::SetKill() {
    strategy_->SetKill();
}
void Kernel::SetHit() {
    strategy_->SetHit();
}
void Kernel::SetMiss() {
    strategy_->SetMiss();
}
void Kernel::SetShipForced(const Coords& coords, uint8_t type, bool rotate) {
    map_->SetShipForced(coords, type, rotate);
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
const bool Kernel::GetType() const {
    return type_;
}


bool Kernel::IsReady() const {
    if (!type_ && (dimension_.Empty() || std::all_of(ships_.begin(), ships_.end(), [](const auto& x) {return x == 0;}))) {
        return false;
    }
    return true;
}
bool Kernel::IsStarted() const {
    return started_;
}
bool Kernel::IsStopped() const {
    return stopped_;
}
bool Kernel::IsWin() const {
    return strategy_->GetShipsSum() == 0;
}
bool Kernel::IsLose() const {
    return map_->GetSize() == 0;
}
bool Kernel::IsStrategySet() const {
    return strategy_ != nullptr;
}

void Kernel::Start() {
    for (uint8_t t = 1; t != 5; ++t) {
        ships_sum_ += t * ships_[t];
    }
    if (strategy_ == nullptr) {
        strategy_ = new CustomStrategy(&dimension_, ships_sum_);
    }
    if (type_) {  // we are master -> generate the dimension.
        Generated generated = strategy_->Generate();
        dimension_ = generated.dimension_;
        ships_ = generated.ships_;
    }
    map_ = new Map(&dimension_, ships_, ships_sum_);
    started_ = true;
}
void Kernel::Stop() {
    stopped_ = true;
}
uint8_t Kernel::HitShip(const Coords& coords) {
    return map_->HitShip(coords);
}
Coords Kernel::Shot() {
    return strategy_->Shot();
}

Kernel::~Kernel() {
    if (map_ != nullptr) {
        delete map_;
    }
    if (strategy_ != nullptr) {
        delete strategy_;
    }
}
