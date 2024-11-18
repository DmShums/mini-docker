// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../include/containerConfig.h"
#include "../include/container.h"
#include "../include/docker.h"
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: mini-docker <config_path>" << std::endl;
        return 1;
    }

    ContainerConfig cfg(argv[1]);
    // Container container(cfg);
    // container.run();

    Docker docker;

    docker.create(cfg);

    docker.list();
    docker.remove(0);

    return 0;
}
