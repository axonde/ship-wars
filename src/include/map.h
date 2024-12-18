#pragma once
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/functional/hash.hpp>
#include <cstdint>

class Map {
public:
    
private:
    boost::unordered_flat_map<std::pair<uint64_t, uint64_t>, bool> map_;
};
