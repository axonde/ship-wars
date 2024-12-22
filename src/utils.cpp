#include "include/utils.h"

void Out::Welcome() {
    std::cout << "🏴‍☠️ Welcome to Ship Wars!\n";
}

void Out::ErrorKernelIsOff() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " The game is not lunch.\n" << RESET;
}
void Out::WarningKernelIsAlreadyStopped() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are already stopped the game. Restart, if you want to replay.\n" << RESET;
}

void Out::ErrorSet() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set width, height, count, strategy, and result.\n" << RESET;
}

void Out::ErrorGet() {
    std::cout << RED << "[ERROR]" << WHITE << " Can't get the given parameters.\n" << RESET;
}

void Out::ErrorPlayerIsSet() {
    std::cout << RED << "[ERROR]" << WHITE << " The player is already set.\n" << RESET;
}
void Out::ErrorPlayerIsUnset() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You aren't able to set any game settings while the type of the player is unset.\n" << RESET;
}
void Out::ErrorPlayerIncorrect() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set master or slave player's type.\n" << RESET;
}

void Out::ErrorStrategy() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set ordered or custom stategy.\n" << RESET;
}
void Out::ErrorStrategyIsAlreadySet() {
    std::cout << RED << "[ERROR]" << WHITE << " You already set the strategy. If you want to change it, restart the program :(\n" << RESET;
}

void Out::ErrorResult() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to miss, hit, or kill opponent.\n" << RESET;
}
void Out::ErrorUnknowCmd() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " Unknow command.\n" << RESET;
}
