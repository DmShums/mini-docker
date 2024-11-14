#ifndef MINI_DOCKER_DOCKER_H
#define MINI_DOCKER_DOCKER_H

#include <cstdlib>
#include <memory>
#include <vector>
#include "container.h"

#define NO_LISTENED_CONTAINER -1

class Docker {
private:
    std::vector<std::unique_ptr<Container>> containers;
    size_t listened_container = NO_LISTENED_CONTAINER;

public:
    void create(const ContainerConfig& cfg);
    void run(size_t idx);
    void kill(size_t idx);
    void listen(size_t idx);
    void detach();
    void list();
    void stop(size_t idx);
    void resume(size_t idx);
};


#endif //MINI_DOCKER_DOCKER_H
