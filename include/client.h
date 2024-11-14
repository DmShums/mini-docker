#ifndef MINI_DOCKER_CLIENT_H
#define MINI_DOCKER_CLIENT_H


#include <cstdio>
#include <string>
#include <vector>

#include "docker.h"

class Client {
private:
    Docker docker;
public:
    void executeCommand(size_t argc, const std::vector<std::string> &argv);
    std::string readCommand();
};

#endif //MINI_DOCKER_CLIENT_H
