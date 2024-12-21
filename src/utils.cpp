#include "include/utils.h"

void Help::ErrorSet() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set width, height, count, strategy, and result.\n" << RESET;
}
void Help::ErrorPlayerIsSet() {
    std::cout << RED << "[ERROR]" << WHITE << " The player is already set.\n" << RESET;
}
void Help::ErrorPlayerIsUnset() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You aren't able to set any game settings while the type of the player is unset.\n" << RESET;
}
void Help::ErrorPlayerIncorrect() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set master or slave player's type.\n" << RESET;
}
void Help::ErrorStrategy() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set ordered or custom stategy.\n" << RESET;
}
void Help::ErrorResult() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to miss, hit, or kill opponent.\n" << RESET;
}
void Help::ErrorUnknowCmd() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " Unknow command.\n" << RESET;
}
void Help::ErrorStrategyParsing() {
    std::cout << RED << "[ERROR]" << WHITE << " Try to set inexistable ship's type.\n" << RESET;
}
void Help::ErrorWhileGetting() {
    std::cout << RED << "[ERROR]" << WHITE << " Can't get the given parameters.\n" << RESET;
}
void Help::ErrorKernelOff() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " The game is not lunch.\n" << RESET;
}
