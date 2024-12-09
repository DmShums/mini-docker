#ifndef MINI_DOCKER_DOCKER_H
#define MINI_DOCKER_DOCKER_H

#include <cstdlib>
#include <memory>
#include <vector>
#include "container.h"
#include <poll.h>

#define NO_LISTENED_CONTAINER (-1)
#define BUF_SIZE 1024

class Docker {
private:
    std::vector<std::unique_ptr<Container>> containers;
    size_t listened_container = NO_LISTENED_CONTAINER;

    bool checkIdx(size_t idx);
    void setnonblocking(int fd);
public:
    void create(const ContainerConfig& cfg);
    void remove(size_t idx);
    void run(size_t idx, bool toAttach);
    void kill(size_t idx);
    void attach(size_t idx, bool sendMsg);
    void list();
    void stop(size_t idx);
    void resume(size_t idx);
};


#endif //MINI_DOCKER_DOCKER_H
