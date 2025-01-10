#include "include/processor.h"

void Processor::Run(bool is_verbose) {
    out_ = is_verbose ? new Verbose : new Out;
    out_->Welcome();

    std::string cmd;
    std::vector<std::string> s_cmd;
    while (true) {
        std::getline(std::cin, cmd);
        s_cmd = Split(cmd, ' ');

        if (s_cmd[0] == "exit") {
            break;
        } else if (s_cmd[0] == "ping") {
            Test();
        }

        // CREATE
        else if (s_cmd[0] == "create" && s_cmd.size() > 1) {
            Create(s_cmd);
        }

        else if (kernel_ == nullptr) {
            out_->ErrorPlayerIsUnset();
            continue;
        }

        // SETTERS
        else if (s_cmd[0] == "set" && s_cmd.size() > 2) {
            Set(s_cmd);
        }
        
        // GETTERS
        else if (s_cmd[0] == "get" && s_cmd.size() > 1) {
            Get(s_cmd);
        }

        // LOAD
        else if (s_cmd[0] == "load" && s_cmd.size() > 1) {
            Load(s_cmd);
        }

        // START
        else if (s_cmd[0] == "start") {
            Start();
        }

        else if (!kernel_->IsStarted()) {
            out_->ErrorKernelIsOff();
            continue;
        }

        // DUMP
        else if (s_cmd[0] == "dump" && s_cmd.size() > 1) {
            Dump(s_cmd);
        }

        // STOP
        else if (s_cmd[0] == "stop") {
            Stop();
        }

        // STATUS
        else if (s_cmd[0] == "finished") {
            IsFinished();
        } else if (s_cmd[0] == "win") {
            IsWin();
        } else if (s_cmd[0] == "lose") {
            IsLose();
        }

        // PRINT (custom)
        else if (s_cmd[0] == "print") {
            out_->PrintMap(kernel_);
        }

        else if (kernel_->IsStopped()) {
            out_->WarningKernelIsAlreadyStopped();
            continue;
        }

        // SHOTS
        else if (s_cmd[0] == "shot" && s_cmd.size() > 2 && IsNumber(s_cmd[1]) && IsNumber(s_cmd[2]) && kernel_->IsStarted()) {
            HitShip(s_cmd);
        } else if (s_cmd[0] == "shot") {
            Shot();
        }

        else {
            out_->ErrorUnknowCmd();
        }
    }
}

void Processor::Test() const {
    std::cout << "pong\n";
}

void Processor::Create(const std::vector<std::string>& s_cmd) {
    if (kernel_ != nullptr) {
        out_->ErrorPlayerIsSet();
        std::cout << "failed\n";
        return;
    }
    if (s_cmd[1] == "master") {
        kernel_ = new Kernel(1);
    } else if (s_cmd[1] == "slave") {
        kernel_ = new Kernel(0);
    } else {
        out_->ErrorPlayerIncorrect();
        std::cout << "failed\n";
        return;
    }
    std::cout << "ok\n";
}

void Processor::Set(const std::vector<std::string>& s_cmd) {
    if (s_cmd[1] == "result") {
        if (!kernel_->IsStarted() || kernel_->IsStopped()) {
            out_->ErrorResult();
            return;
        } else if (s_cmd[2] == "kill") {
            kernel_->SetKill();
        } else if (s_cmd[2] == "hit") {
            kernel_->SetHit();
        } else if (s_cmd[2] == "miss") {
            kernel_->SetMiss();
        }
        if (s_cmd[2] == "miss" || s_cmd[2] == "hit" || s_cmd[2] == "kill") {
            std::cout << "ok\n";
        } else {
            out_->ErrorResultCmd();
        }
    }

    else if (kernel_->IsStarted() || kernel_->IsStopped()) {
        out_->ErrorSet();
    }

    else if (s_cmd[1] == "strategy") {
        if (s_cmd[2] == "ordered") {
            kernel_->SetOrderedStrategy();
            std::cout << "ok\n";
        } else if (s_cmd[2] == "custom") {
            kernel_->SetCustomStrategy();
            std::cout << "ok\n";
        } else { out_->ErrorStrategy(); }
    }

    else if ((s_cmd[1] == "width" || s_cmd[1] == "height" || s_cmd[1] == "count") && !kernel_->GetType()) {
        if (!IsNumber(s_cmd[2]) || !(!(s_cmd.size() > 3) || IsNumber(s_cmd[3]))) {
            std::cout << "failed\n";
            return;
        }
        if (s_cmd[1] == "width") {
            std::cout << (kernel_->SetWidth(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
        } else if (s_cmd[1] == "height") {
            std::cout << (kernel_->SetHeight(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
        } else if (s_cmd[1] == "count" && s_cmd.size() > 3) {
            std::cout << (kernel_->SetCount(std::stoi(s_cmd[2]), std::stoll(s_cmd[3])) ? "ok\n" : "failed\n");
        } else { std::cout << "failed\n"; }
    }
    
    else { out_->ErrorSet(); }
}

void Processor::Get(const std::vector<std::string>& s_cmd) const {
    if (s_cmd[1] == "width") {
        std::cout << kernel_->GetWidth() << '\n';
    } else if (s_cmd[1] == "height") {
        std::cout << kernel_->GetHeight() << '\n';
    } else if (s_cmd[1] == "count" && s_cmd.size() > 2 && IsNumber(s_cmd[2]) && "1" <= s_cmd[2] && s_cmd[2] <= "4") {
        std::cout << kernel_->GetCount(std::stoi(s_cmd[2])) << '\n';
    } else {
        out_->ErrorGet();
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
        out_->ErrorKernelIsOff();
        std::cout << "failed\n";
        return;
    }
    if (kernel_->IsStopped()) {
        out_->WarningKernelIsAlreadyStopped();
        std::cout << "failed\n";
        return;
    }
    kernel_->Stop();
    std::cout << "ok\n";
}

void Processor::HitShip(const std::vector<std::string>& s_cmd) {
    if (kernel_->IsWin()) {
        out_->AbortAlreadyWin();
        return;
    }
    if (kernel_->IsLose()) {
        out_->AbortAlreadyLose();
        return;
    }
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
    if (kernel_->IsWin()) {
        out_->AbortAlreadyWin();
        return;
    }
    if (kernel_->IsLose()) {
        out_->AbortAlreadyLose();
        return;
    }
    Coords shot = kernel_->Shot();
    std::cout << shot.x << ' ' << shot.y << '\n';
}

void Processor::Dump(const std::vector<std::string>& s_cmd) {
    if (Parser::Dump(kernel_, s_cmd[1])) {
        std::cout << "ok\n";
    } else {
        std::cout << "failed\n";
    }
}
void Processor::Load(const std::vector<std::string>& s_cmd) {
    if (!kernel_->GetType() && Parser::Load(kernel_, s_cmd[1])) {
        std::cout << "ok\n";
        out_->WarningLoad();
    } else {
        if (kernel_->GetType()) {
            out_->ErrorLoadMasterPlayer();
        }
        std::cout << "failed\n";
    }
}

void Processor::IsWin() const {
    std::cout << (kernel_->IsWin() ? "yes\n" : "no\n");
}
void Processor::IsLose() const {
    std::cout << (kernel_->IsLose() ? "yes\n" : "no\n");
}
void Processor::IsFinished() const {
    std::cout << (kernel_->IsWin() || kernel_->IsLose() ? "yes\n" : "no\n");
}

Processor::~Processor() {
    if (kernel_ != nullptr) {
        delete kernel_;
    }
    if (out_ != nullptr) {
        delete out_;
    }
}
