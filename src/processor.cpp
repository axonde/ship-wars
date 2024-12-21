#include "include/processor.h"

void Processor::Run() {
    std::string cmd;
    std::vector<std::string> s_cmd;
    while (true) {
        std::getline(std::cin, cmd);
        s_cmd = Split(cmd, ' ');

        std::string RED = "\e[1;31m";
        std::string YELLOW = "\e[1;33m";
        std::string PURPLE = "\e[1;35m";
        std::string WHITE = "\e[97m";
        std::string RESET = "\e[0m";

        if (s_cmd[0] == "exit") {
            break;
        } else if (s_cmd[0] == "ping") {
            Test();
        } 

        // CREATE
        else if (s_cmd[0] == "create" && s_cmd.size() > 1) {
            if (!Create(s_cmd)) continue;
        }

        else if (kernel_ == nullptr) {
            Help::ErrorPlayerIsUnset();
            continue;
        }
        
        // START & STOP
        else if (s_cmd[0] == "start") {
            std::cout << (!Start() ? "failed\n" : "ok\n");
        } else if (s_cmd[0] == "stop") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " Stop the game.\n" << RESET;
        }
        
        // SETTERS
        else if (s_cmd[0] == "set" && s_cmd.size() > 2) {
            if (!Set(s_cmd)) continue;
        }
        
        // GETTERS
        else if (s_cmd[0] == "get" && s_cmd.size() > 1) {
            Get(s_cmd);
        }
        
        // SHOTS
        else if (s_cmd[0] == "shot" && s_cmd.size() > 2 && IsNumber(s_cmd[1]) && IsNumber(s_cmd[2]) && kernel_->IsStarted()) {
            if (uint8_t shot = kernel_->Shot(
                {static_cast<uint64_t>(std::stoll(s_cmd[1])), 
                static_cast<uint64_t>(std::stoll(s_cmd[2]))
            }); shot == 0) std::cout << "miss\n";
            else if (shot == 1) std::cout << "hit\n";
            else std::cout << "kill\n";
        } else if (s_cmd[0] == "shot") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the X Y coords of the shot.\n" << RESET;
        }
        
        // STATUS
        else if (s_cmd[0] == "finished") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game finished: yes/no\n" << RESET;
        } else if (s_cmd[0] == "win") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game, are we won: yes/no.\n" << RESET;
        } else if (s_cmd[0] == "lose") {
            std::cout << (kernel_->IsLoose() ? "yes\n" : "no\n");
        }
        
        // PARSE
        else if (s_cmd[0] == "dump" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " dump the created map to: " << s_cmd[1] << "\n" << RESET;
        } else if (s_cmd[0] == "load" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " uploading the game setting from: " << s_cmd[1] << "\n" << RESET;
        }
        
        else {
            Help::ErrorUnknowCmd();
        }
    }
}

void Processor::Test() {
    std::cout << "pong\n";
}

bool Processor::Create(const std::vector<std::string>& s_cmd) {
    if (kernel_ != nullptr) {
        Help::ErrorPlayerIsSet();
        return false;
    }
    if (s_cmd[1] == "master") {
        kernel_ = new Kernel(1);
    } else if (s_cmd[1] == "slave") {
        kernel_ = new Kernel(0);
    } else {
        Help::ErrorPlayerIncorrect();
        return false;
    }
    std::cout << "ok\n";
    return true;
}

bool Processor::Set(const std::vector<std::string>& s_cmd) {
    if (kernel_->IsStarted()) {
        return false;
    }

    if (s_cmd[1] == "strategy") {
        if (s_cmd[2] == "ordered") {
            if (kernel_->SetStrategy(0)) std::cout << "ok\n";
            else Help::ErrorStrategyParsing();
        } else if (s_cmd[2] == "custom") {
            if (kernel_->SetStrategy(1)) std::cout << "ok\n";
            else Help::ErrorStrategyParsing();
        } else {
            Help::ErrorStrategy();
        }
    }
    
    else if (s_cmd[1] == "result") {
        if (s_cmd[2] == "miss") {
            std::cout << Help::YELLOW << "[PLUG]" << Help::WHITE << " miss.\n" << Help::RESET;
        } else if (s_cmd[2] == "hit") {
            std::cout << Help::YELLOW << "[PLUG]" << Help::WHITE << " hit.\n" << Help::RESET;
        } else if (s_cmd[2] == "kill") {
            std::cout << Help::YELLOW << "[PLUG]" << Help::WHITE << " kill.\n" << Help::RESET;
        } else {
            Help::ErrorResult();
        }
    }

    else if (s_cmd[1] == "width" || s_cmd[1] == "height" || s_cmd[1] == "count") {
        if (!IsNumber(s_cmd[2]) || !(!(s_cmd.size() > 3) || IsNumber(s_cmd[3]))) {
            std::cout << "failed\n";
            return false;
        }
        if (s_cmd[1] == "width") {
            std::cout << (kernel_->SetWidth(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
        } else if (s_cmd[1] == "height") {
            std::cout << (kernel_->SetHeight(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
        } else if (s_cmd[1] == "count" && s_cmd.size() > 3) {
            std::cout << (kernel_->SetCount(std::stoi(s_cmd[2]), std::stoll(s_cmd[3])) ? "ok\n" : "failed\n");
        } else { std::cout << "failed\n"; }
    }
    
    else {
        Help::ErrorSet();
    }
    return true;
}

void Processor::Get(const std::vector<std::string>& s_cmd) {
    if (s_cmd[1] == "width") {
        std::cout << kernel_->GetWidth() << '\n';
    } else if (s_cmd[1] == "height") {
        std::cout << kernel_->GetHeight() << '\n';
    } else if (s_cmd[1] == "count" && s_cmd.size() > 2 && IsNumber(s_cmd[2])) {
        std::cout << kernel_->GetCount(std::stoi(s_cmd[2])) << '\n';
    } else {
        Help::ErrorWhileGetting();
    }
}

bool Processor::Start() {
    if (!kernel_->IsReady() || kernel_->IsStarted()) {
        return false;
    }
    kernel_->Start();
    return true;
}

uint8_t Processor::Shot(const Coords& coords) {
    return kernel_->Shot(coords);
}

Processor::~Processor() {
    if (kernel_ != nullptr) {
        delete kernel_;  // we allocate this memory in proccesor run.
    }
}
