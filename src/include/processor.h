#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "kernel.h"
#include "utils.h"

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
    static bool IsNumber(const std::string &s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    }

    ~Processor();
private:
    Kernel* kernel_ = nullptr;
};
