// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "containerConfig.h"
#include "container.h"
#include "docker.h"
#include "client.h"
#include <iostream>
#include <sstream>
#include <iterator>


int main(int argc, char* argv[]) {
    Client client;
    std::string command;
    std::cout << "started with: " << getpid() << std::endl;

    while (true) {
        command = client.readCommand();

        std::istringstream iss(command);
        std::vector<std::string> argv((std::istream_iterator<std::string>(iss)), 
                                      std::istream_iterator<std::string>());
        if (argv.empty()) {
            continue;
        }

        if (argv[0] == "exit") {
            std::cout << "Exiting mini-docker client." << std::endl;
            break;
        }

        client.executeCommand(argv.size(), argv);
    }
    return 0;
}

