#pragma once
#include <iostream>

namespace Help {
    inline static char RED[8] = "\e[1;31m";
    inline static char YELLOW[8] = "\e[1;33m";
    inline static char PURPLE[8] = "\e[1;35m";
    inline static char WHITE[6] = "\e[97m";
    inline static char RESET[5] = "\e[0m";

    void ErrorSet();
    void ErrorPlayerIsSet();
    void ErrorPlayerIsUnset();
    void ErrorPlayerIncorrect();
    void ErrorStrategy();
    void ErrorResult();
    void ErrorUnknowCmd();
    void ErrorStrategyParsing();
    void ErrorWhileGetting();
    void ErrorKernelOff();
}