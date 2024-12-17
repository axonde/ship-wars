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
        
        else if (s_cmd[0] == "create" && s_cmd.size() > 1) {
            if (kernel_ != nullptr) {
                std::cout << RED << "[ERROR]" << WHITE << " The player is already set.\n" << RESET;
                continue;
            }
            if (s_cmd[1] == "master") {
                kernel_ = new Kernel(1);
            } else if (s_cmd[1] == "slave") {
                kernel_ = new Kernel(0);
            } else {
                std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set master or slave player's type.\n" << RESET;
                continue;
            }
            std::cout << "ok\n";
        } 
        
        else if (s_cmd[0] == "start") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " Start the game.\n" << RESET;
        } else if (s_cmd[0] == "stop") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " Stop the game.\n" << RESET;
        }
        
        else if (s_cmd[0] == "set" && s_cmd.size() > 2) {
            if (kernel_ == nullptr) {
                std::cout << PURPLE << "[WARNING]" << WHITE << " You aren't able to set any game setting while the type of the player is unset.\n" << RESET;
                continue;
            }
            if (!IsNumber(s_cmd[2]) || !(!(s_cmd.size() > 3) || IsNumber(s_cmd[3]))) {
                std::cout << "failed\n";
                continue;
            }

            if (s_cmd[1] == "width") {
                std::cout << (kernel_->SetWidth(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
            } else if (s_cmd[1] == "height") {
                std::cout << (kernel_->SetHeight(std::stoll(s_cmd[2])) ? "ok\n" : "failed\n");
            } else if (s_cmd[1] == "count" && s_cmd.size() > 3) {
                std::cout << (kernel_->SetCount(std::stoi(s_cmd[2]), std::stoll(s_cmd[3])) ? "ok\n" : "failed\n");
            }
            
            else if (s_cmd[1] == "strategy") {
                if (s_cmd[2] == "ordered") {
                    std::cout << YELLOW << "[PLUG]" << WHITE << " Set ordered strategy.\n" << RESET;
                } else if (s_cmd[2] == "custom") {
                    std::cout << YELLOW << "[PLUG]" << WHITE << " Set custom strategy.\n" << RESET;
                } else {
                    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set ordered or custom stategy.\n" << RESET;
                }
            }
            
            else if (s_cmd[1] == "result") {
                if (s_cmd[2] == "miss") {
                    std::cout << YELLOW << "[PLUG]" << WHITE << " miss.\n" << RESET;
                } else if (s_cmd[2] == "hit") {
                    std::cout << YELLOW << "[PLUG]" << WHITE << " hit.\n" << RESET;
                } else if (s_cmd[2] == "kill") {
                    std::cout << YELLOW << "[PLUG]" << WHITE << " kill.\n" << RESET;
                } else {
                    std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to miss, hit, or kill opponent.\n" << RESET;
                }
            } 
            
            else {
                std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set width, height, count, strategy, and result.\n" << RESET;
            }
        }
        
        else if (s_cmd[0] == "get" && s_cmd.size() > 1) {
            if (s_cmd[1] == "width") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " return the width.\n" << RESET;
            } else if (s_cmd[1] == "height") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " return the height.\n" << RESET;
            } else if (s_cmd[1] == "count" && s_cmd.size() > 2) {
                std::cout << YELLOW << "[PLUG]" << WHITE << " return the number of " << std::stoi(s_cmd[2]) << " ship's type.\n" << RESET;
            }
        }
        
        else if (s_cmd[0] == "shot" && s_cmd.size() > 2) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " striking a blow on " << s_cmd[1] << ' ' << s_cmd[2] << ".\n" << RESET;
        } else if (s_cmd[0] == "shot") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the X Y coords of the shot.\n" << RESET;
        }
        
        else if (s_cmd[0] == "finished") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game finished: yes/no\n" << RESET;
        } else if (s_cmd[0] == "win") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game won: yes/no.\n" << RESET;
        } else if (s_cmd[0] == "lose") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " return the status have the game lost: yes/no.\n" << RESET;
        }
        
        else if (s_cmd[0] == "dump" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " dump the created map to: " << s_cmd[1] << "\n" << RESET;
        } else if (s_cmd[0] == "load" && s_cmd.size() > 1) {
            std::cout << YELLOW << "[PLUG]" << WHITE << " uploading the game setting from: " << s_cmd[1] << "\n" << RESET;
        }
        
        else {
            std::cout << PURPLE << "[WARNING]" << WHITE << " Unknow command.\n" << RESET;
        }
    }
}

void Processor::Test() {
    std::cout << "pong\n";
}

Processor::~Processor() {
    if (kernel_ != nullptr) {
        delete kernel_;  // we allocate this memory in proccesor run.
    }
}
