#include "include/processor.h"

void Processor::Run() {
    std::string cmd;
    std::vector<std::string> s_cmd;
    while (true) {
        std::getline(std::cin, cmd);
        s_cmd = Split(cmd, ' ');

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
            if (s_cmd[1] == "master") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " Set the master player.\n" << RESET;
            } else if (s_cmd[1] == "slave") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " Set the slave player.\n" << RESET;
            } else {
                std::cout << PURPLE << "[WARNING]" << WHITE << " You are only able to set master or slave player's type.\n" << RESET;
            }
        } 
        
        else if (s_cmd[0] == "start") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " Start the game.\n" << RESET;
        } else if (s_cmd[0] == "stop") {
            std::cout << YELLOW << "[PLUG]" << WHITE << " Stop the game.\n" << RESET;
        }
        
        else if (s_cmd[0] == "set" && s_cmd.size() > 2) {
            if (s_cmd[1] == "width") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " Set width = " << std::stoll(s_cmd[2]) << '\n' << RESET;
            } else if (s_cmd[1] == "height") {
                std::cout << YELLOW << "[PLUG]" << WHITE << " Set height = " << std::stoll(s_cmd[2]) << '\n' << RESET;
            } else if (s_cmd[1] == "count" && s_cmd.size() > 3) {
                std::cout << YELLOW << "[PLUG]" << WHITE << " Set count of " << std::stoi(s_cmd[2]) << " ship's type for " << std::stoll(s_cmd[3]) << " items." << '\n' << RESET;
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
