#pragma once
#include <iostream>
#include "kernel.h"

class Out {
public:
    inline static char RED[8] = "\e[1;31m";
    inline static char YELLOW[8] = "\e[1;33m";
    inline static char PURPLE[8] = "\e[1;35m";
    inline static char WHITE[8] = "\e[1;97m";
    inline static char RESET[5] = "\e[0m";

    virtual void Welcome() {}
    
    virtual void ErrorKernelIsOff() {}
    virtual void WarningKernelIsAlreadyStopped() {}

    virtual void ErrorSet() {}

    virtual void ErrorGet() {}
    virtual void ErrorEarlierMasterGetting() {}

    virtual void ErrorResult() {}
    virtual void ErrorResultCmd() {}

    virtual void ErrorPlayerIsSet() {}
    virtual void ErrorPlayerIsUnset() {}
    virtual void ErrorPlayerIncorrect() {}

    virtual void ErrorStrategy() {}
    virtual void ErrorStrategyIsAlreadySet() {}

    virtual void WarningLoad() {}
    virtual void ErrorLoadMasterPlayer() {}

    virtual void AbortAlreadyWin();
    virtual void AbortAlreadyLose();

    virtual void ErrorUnknowCmd() {}

    void PrintMap(const Kernel* kernel);
};

class Verbose : public Out {
    void Welcome() override;
    
    void ErrorKernelIsOff() override;
    void WarningKernelIsAlreadyStopped() override;

    void ErrorSet() override;

    void ErrorGet() override;
    void ErrorEarlierMasterGetting() override;

    void ErrorResult() override;
    void ErrorResultCmd() override;

    void ErrorPlayerIsSet() override;
    void ErrorPlayerIsUnset() override;
    void ErrorPlayerIncorrect() override;

    void ErrorStrategy() override;
    void ErrorStrategyIsAlreadySet() override;

    void WarningLoad() override;
    void ErrorLoadMasterPlayer() override;

    void AbortAlreadyWin() override;
    void AbortAlreadyLose() override;

    void ErrorUnknowCmd() override;
};
