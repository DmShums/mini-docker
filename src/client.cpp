// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../include/client.h"
#include "../include/containerConfig.h"
#include "../include/container.h"
#include "../include/docker.h"

void Client::executeCommand(size_t argc, const std::vector<std::string>& argv) {
    if (argv.empty()) {
        std::cerr << "No command provided!" << std::endl;
        return;
    }

    const std::string& cmd = argv[0];
    try {
        if (cmd == "create") {
            if (argc < 2) {
                std::cerr << "Usage: create <config_path>" << std::endl;
                return;
            }

            std::string configPath = argv[1];
            ContainerConfig cfg(configPath);
            docker.create(cfg);
        } else if (cmd == "list") {
            docker.list();
        } else if (cmd == "run") {
            if (argc < 2) {
                std::cerr << "Usage: run <index>" << std::endl;
                return;
            }
            docker.run(std::stoul(argv[1]));
        } else if (cmd == "remove") {
            if (argc < 2) {
                std::cerr << "Usage: remove <index>" << std::endl;
                return;
            }
            docker.remove(std::stoul(argv[1]));
        } else if (cmd == "stop") {
            if (argc < 2) {
                std::cerr << "Usage: stop <index>" << std::endl;
                return;
            }
            docker.stop(std::stoul(argv[1]));
        } else if (cmd == "resume") {
            if (argc < 2) {
                std::cerr << "Usage: resume <index>" << std::endl;
                return;
            }
            docker.resume(std::stoul(argv[1]));
        } else {
            std::cerr << "Unknown command: " << cmd << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error executing command: " << e.what() << std::endl;
    }
}

std::string Client::readCommand() {
    std::cout << "mini-docker> ";
    std::string line;
    std::getline(std::cin, line);
    return line;
}
