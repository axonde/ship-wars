#include "include/processor.h"

void Processor::Run() {
    Out::Welcome();

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
            if (!Create(s_cmd)) {
                continue;
            }
        }

        else if (kernel_ == nullptr) {
            Out::ErrorPlayerIsUnset();
            continue;
        }
        
        // START & STOP
        else if (s_cmd[0] == "start") {
            Start();
        } else if (s_cmd[0] == "stop") {
            Stop();
        }
        
        // SETTERS
        else if (s_cmd[0] == "set" && s_cmd.size() > 2) {
            if (!Set(s_cmd)) {
                continue;
            }
        }
        
        // GETTERS
        else if (s_cmd[0] == "get" && s_cmd.size() > 1) {
            Get(s_cmd);
        }

        // PARSE
        else if (s_cmd[0] == "dump" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " dump the created map to: " << s_cmd[1] << "\n" << RESET;
        } else if (s_cmd[0] == "load" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " uploading the game setting from: " << s_cmd[1] << "\n" << RESET;
        }

        else if (!kernel_->IsStarted()) {
            Out::ErrorKernelIsOff();
            continue;
        }
        
        // STATUS
        else if (s_cmd[0] == "finished") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game finished: yes/no\n" << RESET;
        } else if (s_cmd[0] == "win") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game, are we won: yes/no.\n" << RESET;
        } else if (s_cmd[0] == "lose") {
            IsLose();
        }

        else if (kernel_->IsStopped()) {
            Out::WarningKernelIsAlreadyStopped();
            continue;
        }

        // SHOTS
        else if (s_cmd[0] == "shot" && s_cmd.size() > 2 && IsNumber(s_cmd[1]) && IsNumber(s_cmd[2]) && kernel_->IsStarted()) {
            HitShip(s_cmd);
        } else if (s_cmd[0] == "shot") {
            Shot();
        }
        
        else {
            Out::ErrorUnknowCmd();
        }
    }
}

void Processor::Test() const {
    std::cout << "pong\n";
}

bool Processor::Create(const std::vector<std::string>& s_cmd) {
    if (kernel_ != nullptr) {
        Out::ErrorPlayerIsSet();
        return false;
    }
    if (s_cmd[1] == "master") {
        kernel_ = new Kernel(1);
    } else if (s_cmd[1] == "slave") {
        kernel_ = new Kernel(0);
    } else {
        Out::ErrorPlayerIncorrect();
        return false;
    }
    std::cout << "ok\n";
    return true;
}

bool Processor::Set(const std::vector<std::string>& s_cmd) {
    if (s_cmd[1] == "result") {
        if (!kernel_->IsStarted() || kernel_->IsStopped()) {
            Out::ErrorResult();
            return false;
        }
        else if (s_cmd[2] == "hit" || s_cmd[2] == "kill") {
            kernel_->SetShot();
        }
        if (s_cmd[2] == "miss" || s_cmd[2] == "hit" || s_cmd[2] == "kill") {
            std::cout << "ok\n";
        } else {
            Out::ErrorResult();
            return false;
        }
    }

    else if (kernel_->IsStarted() || kernel_->IsStopped()) {
        Out::ErrorSet();
        return false;
    }

    else if (s_cmd[1] == "strategy") {
        if (kernel_->IsStrategySet()) {
            Out::ErrorStrategyIsAlreadySet();
        } else if (s_cmd[2] == "ordered") {
            kernel_->SetOrderedStrategy();
            std::cout << "ok\n";
        } else if (s_cmd[2] == "custom") {
            kernel_->SetCustomStrategy();
            std::cout << "ok\n";
        } else {
            Out::ErrorStrategy();
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
        Out::ErrorSet();
    }
    return true;
}

void Processor::Get(const std::vector<std::string>& s_cmd) const {
    if (s_cmd[1] == "width") {
        std::cout << kernel_->GetWidth() << '\n';
    } else if (s_cmd[1] == "height") {
        std::cout << kernel_->GetHeight() << '\n';
    } else if (s_cmd[1] == "count" && s_cmd.size() > 2 && IsNumber(s_cmd[2])) {
        std::cout << kernel_->GetCount(std::stoi(s_cmd[2])) << '\n';
    } else {
        Out::ErrorGet();
    }
}

void Processor::Start() {
    if (!kernel_->IsReady() || kernel_->IsStarted()) {
        std::cout << "failed\n";
        return;
    }
    kernel_->Start();
    std::cout << "ok\n";
}

void Processor::Stop() {
    if (!kernel_->IsStarted()) {
        Out::ErrorKernelIsOff();
        return;
    }
    if (kernel_->IsStopped()) {
        Out::WarningKernelIsAlreadyStopped();
        return;
    }
    kernel_->Stop();
    std::cout << "ok\n";
}

void Processor::HitShip(const std::vector<std::string>& s_cmd) {
    if (uint8_t shot = kernel_->HitShip({
        static_cast<uint64_t>(std::stoll(s_cmd[1])), 
        static_cast<uint64_t>(std::stoll(s_cmd[2]))
    }); shot == 0) {
        std::cout << "miss\n";
    }
    else if (shot == 1) std::cout << "hit\n";
    else std::cout << "kill\n";
}

void Processor::Shot() {
    Coords shot = kernel_->Shot();
    std::cout << shot.x << ' ' << shot.y << '\n';
}

void Processor::IsLose() const {
    std::cout << (kernel_->IsLose() ? "yes\n" : "no\n");
}

Processor::~Processor() {
    if (kernel_ != nullptr) {
        delete kernel_;  // we allocate this memory in proccesor run.
    }
}
