#ifndef MINI_DOCKER_CONTAINER_H
#define MINI_DOCKER_CONTAINER_H

#include "containerConfig.h"
#include "cgroup.h"

#include <sys/mount.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

class Container {
private:
    ContainerConfig cfg;
    CGroup cgroup;

    int isolated_fs = 0;

    int isolate_filesystem();
    void clear_filesystem();
    int isolate_namespaces();

    std::vector<std::string> defaultMountPoints = {
        "/lib:/lib",
    };


public:
    Container() = delete;
    Container(const ContainerConfig& cfg);

    void run();

    ~Container();
};

#endif //MINI_DOCKER_CONTAINER_H
