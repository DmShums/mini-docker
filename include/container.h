#ifndef MINI_DOCKER_CONTAINER_H
#define MINI_DOCKER_CONTAINER_H

#include "containerConfig.h"
#include "cgroup.h"

#include <sys/mount.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

#define PID_NOT_SET (-1)

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
    pid_t procPid;

    Container() = delete;
    Container(const ContainerConfig& cfg);

    void run();
    std::string getName() const {return cfg.name;}
    std::string info() const {
        std::string result = "\tcommand:\t";
        for (const std::string &arg : cfg.argv){
            result += arg + " ";
        }
        result += "\n\tpid:\t" + procPid;
        result += "\n\tnew root:\t" + cfg.new_root + "\n";
    }

    ~Container();
};

#endif //MINI_DOCKER_CONTAINER_H
