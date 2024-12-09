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
#define PIPE_NOT_SET (-1)

#define READ 0
#define WRITE 1

#define ATTACHED_MSG "attach"

class Container {
private:
    ContainerConfig cfg;
    CGroup cgroup;

    int created_fs = false;

    int isolate_filesystem();
    int prepare_filesystem();
    void clear_filesystem();
    int isolate_namespaces();
    int setUpChildIPC(int pipe_to_proc[2], int pipe_from_proc[2], bool waitAttach);

public:
    pid_t procPid;

    int pipeToProc = PIPE_NOT_SET;
    int pipeFromProc = PIPE_NOT_SET;

    Container() = delete;
    Container(const ContainerConfig& cfg);

    void run(bool waitAttach);
    std::string getName() const {return cfg.name;}
    std::string info() const {
        std::string result = "\tcommand:\t";
        for (const std::string &arg : cfg.argv){
            result += arg + " ";
        }
        result += "\n\tpid:\t" + procPid;
        result += "\n\tnew root:\t" + cfg.new_root + "\n";

        return result;
    }

    ~Container();
};

#endif //MINI_DOCKER_CONTAINER_H
