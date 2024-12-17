#include "include/processor.h"

void Processor::Run() {
    while (true) {
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "exit") {
            break;
        } else if (cmd == "ping") {
            Test();
        }
    }
}

void Processor::Test() {
    std::cout << "pong\n";
}
