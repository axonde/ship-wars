#pragma once
#include <iostream>

namespace Out {
    inline static char RED[8] = "\e[1;31m";
    inline static char YELLOW[8] = "\e[1;33m";
    inline static char PURPLE[8] = "\e[1;35m";
    inline static char WHITE[6] = "\e[97m";
    inline static char RESET[5] = "\e[0m";

    void Welcome();
    
    void ErrorKernelIsOff();
    void WarningKernelIsAlreadyStopped();

    void ErrorSet();

    void ErrorGet();
    void ErrorEarlierMasterGetting();

    void ErrorResult();
    void ErrorResultCmd();

    void ErrorPlayerIsSet();
    void ErrorPlayerIsUnset();
    void ErrorPlayerIncorrect();

    void ErrorStrategy();
    void ErrorStrategyIsAlreadySet();

    void ErrorUnknowCmd();
}