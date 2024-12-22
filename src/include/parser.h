#pragma once
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
#include "kernel.h"
#include "map.h"

namespace Parser {
    namespace fs = std::filesystem;
    bool Dump(Kernel* kernel, const std::string& path);
    bool Load(Kernel* kernel, const std::string& path);
}