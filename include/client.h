#ifndef MINI_DOCKER_CLIENT_H
#define MINI_DOCKER_CLIENT_H


#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>


#include "docker.h"

class Client {
private:
    Docker docker;
public:
    void executeCommand(size_t argc, const std::vector<std::string> &argv);
    std::string readCommand();
};

#endif //MINI_DOCKER_CLIENT_H
