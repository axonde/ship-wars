#include "include/utils.h"

void Out::AbortAlreadyWin() {
    std::cout << "failed\n";
}
void Out::AbortAlreadyLose() {
    std::cout << "failed\n";
}

void Out::PrintMap(const Kernel* kernel) {
    std::cout << kernel->GetMap();
}

// VERBOSE
void Verbose::Welcome() {
    std::cout << "🏴‍☠️ Welcome to Ship Wars!\n";
}

void Verbose::ErrorKernelIsOff() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " The game is not lunch 😇\n" << RESET;
}
void Verbose::WarningKernelIsAlreadyStopped() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are already stopped the game. Restart, if you want to replay.\n" << RESET;
}

void Verbose::ErrorSet() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " NO! You are setting somes for master player or try to set count of potatoes.\n" << RESET;
}

void Verbose::ErrorEarlierMasterGetting() {
    std::cout << RED << "[ERROR]" << WHITE << " So... no. I will repeat it once. You can't get smth " << RED << "while the game hasn't started 😡\n" << RESET;
}
void Verbose::ErrorGet() {
    std::cout << RED << "[ERROR]" << WHITE << " 1) You need to have a normal tone. 2) Maybe you try to get avocados(not here pls)\n" << RESET;
}

void Verbose::ErrorResult() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " No, no, no. The game isn't lunch or is stopped and you try to confuse me!\n" << RESET;
}
void Verbose::ErrorResultCmd() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to miss, hit, or kill opponent. (OBVIOUS)\n" << RESET;
}

void Verbose::ErrorPlayerIsSet() {
    std::cout << RED << "[ERROR]" << WHITE << " The player is already set.\n" << RESET;
}
void Verbose::ErrorPlayerIsUnset() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " Ho-ho. You can't do anything without provide me the type of the player 😝\n" << RESET;
}
void Verbose::ErrorPlayerIncorrect() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set master or slave player's type.\n" << RESET;
}

void Verbose::ErrorStrategy() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set ordered or custom stategy.\n" << RESET;
}
void Verbose::ErrorStrategyIsAlreadySet() {
    std::cout << RED << "[ERROR]" << WHITE << " You already set the strategy. If you want to change it, restart the program :(\n" << RESET;
}

void Verbose::WarningLoad() {
    std::cout << PURPLE << "[WARNING]" << RESET << " Please notice, that after the load command the game is " << WHITE << "already started!" << RESET << "\n";
}
void Verbose::ErrorLoadMasterPlayer() {
    std::cout << RED << "[ERROR]" << RESET << " You can't load file while you are a master player.\n";
}

void Verbose::AbortAlreadyWin() {
    std::cout << YELLOW << "[ABORT]" << RESET << " I already win! 🏆🤫🧏\n";
}
void Verbose::AbortAlreadyLose() {
    std::cout << YELLOW << "[ABORT]" << RESET << " You already win.. 😡🤡\n";
}

void Verbose::ErrorUnknowCmd() {
    std::cout << PURPLE << "[WARNING]" << WHITE << " Unknow command.\n" << RESET;
}
