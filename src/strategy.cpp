#include "include/strategy.h"

bool Strategy::SetType(uint8_t type) {
    if (std::find(types_.begin(), types_.end(), type) == types_.end()) {
        return false;
    }
    type_ = type;
    return true;
}
