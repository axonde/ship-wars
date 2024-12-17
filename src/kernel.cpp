#include "include/kernel.h"

Kernel::Kernel(bool type) : type_(type) {}


bool Kernel::SetWidth(uint64_t width) {
    dimension_.width_ = width;
    return true;
}
bool Kernel::SetHeight(uint64_t height) {
    dimension_.height_ = height;
    return true;
}
bool Kernel::SetCount(uint8_t type, uint64_t count) {
    if (type < 0 || type > 3) {
        return false;
    }
    ships_[type] = count;
    return true;
}