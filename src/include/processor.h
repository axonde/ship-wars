#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "map.h"
#include "kernel.h"
#include "utils.h"

class Processor {
public:
    void Run();
    void Test() const;
    bool Create(const std::vector<std::string>& s_cmd);
    bool Set(const std::vector<std::string>& s_cmd);
    void Get(const std::vector<std::string>& s_cmd) const;
    void Start();
    void Stop();
    void HitShip(const std::vector<std::string>& s_cmd);
    void Shot();

    void IsLose() const;

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
