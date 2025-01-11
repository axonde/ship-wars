#include <boost/functional/hash.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include "include/parser.h"

bool Parser::Dump(Kernel* kernel, const std::string& path) {
    fs::path file_path = path;
    std::fstream file{file_path, file.out};
    
    if (!file.is_open()) {
        return false;
    }

    file << kernel->GetWidth() << ' ' << kernel->GetHeight() << '\n';
    UnorderedSet map = kernel->GetMap().GetMap();
    UnorderedMap ships;  // ships[i] = 12 -> 0 / 1 - type (x / 10), 2 -> length (x % 10)
    while (!map.empty()) {
        std::vector<Coords> ship;
        Coords coord = *map.begin();

        UnorderedSet::iterator top, right, bottom, left;
        bool isTop, isRight, isBottom, isLeft;

        std::function<void()> resetSides = [&]() {
            top = map.find({coord.x, coord.y - 1});
            right = map.find({coord.x + 1, coord.y});
            bottom = map.find({coord.x, coord.y + 1});
            left = map.find({coord.x - 1, coord.y});
            isTop = top != map.end() && !coord.IsTouchingTop();
            isRight = right != map.end() && !coord.IsTouchingRight({kernel->GetWidth(), kernel->GetHeight()});
            isBottom = bottom != map.end() && !coord.IsTouchingBottom({kernel->GetWidth(), kernel->GetHeight()});
            isLeft = left != map.end() && !coord.IsTouchingLeft();
        };

        bool type = 0;
        resetSides();
        while (isTop || isLeft) {
            if (isTop) {
                coord = *top;
                type = 1;
            } else if (isLeft) {
                coord = *left;
            }
            resetSides();
        }
        ship.push_back(coord);
        map.erase(coord);
        while (isRight || isBottom) {
            if (isRight) {
                coord = *right;
                ship.push_back(coord);
                map.erase(coord);
            } else if (isBottom) {
                coord = *bottom;
                ship.push_back(coord);
                map.erase(coord);
                type = 1;
            }
            resetSides();
        }
        ships.insert({*ship.begin(), type * 10 + ship.size()});
    }

    for (const auto& [coord, info] : ships) {
        file << info % 10 << ' ' << (info / 10 == 0 ? 'h' : 'v') << ' ' << coord.x << ' ' << coord.y << '\n';
    }

    file.close();
    return true;
}

bool Parser::Load(Kernel* kernel, const std::string& path) {
    fs::path file_path = path;
    std::fstream file{file_path, file.in};
    
    if (!file.is_open()) {
        return false;
    }

    uint64_t width;
    uint64_t height;
    file >> width >> height;
    kernel->SetWidth(width);
    kernel->SetHeight(height);

    kernel->Start();

    int type;
    char rotate;
    uint64_t x;
    uint64_t y;
    while (file >> type >> rotate >> x >> y) {
        kernel->SetShipForced({x, y}, type, rotate == 'v' ? true : false);
    }

    return true;
}
