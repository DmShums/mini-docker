#ifndef MINI_DOCKER_CONTAINER_H
#define MINI_DOCKER_CONTAINER_H

#include "containerConfig.h"

class Container {
private:

public:
    Container() = delete;
    Container(const ContainerConfig& cfg);

    void run();
};


#endif //MINI_DOCKER_CONTAINER_H
