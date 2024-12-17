#pragma once
#include <iostream>
#include <string>
#include <vector>

class Processor {
public:
    void Run();
    void Test();

    static std::vector<std::string> Split(const std::string& str, char delimiter) {
        std::vector<std::string> arr;
        size_t l = 0;
        size_t n = str.size();
        for (size_t r = 0; r != n + 1; ++r) {
            if (r == n || str[r] == delimiter) {
                arr.push_back(str.substr(l, r - l));
                l = r + 1;
            }
        }
        return arr;
    }
};
