#ifndef MINI_DOCKER_CGROUP_H
#define MINI_DOCKER_CGROUP_H

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#define CGROUP_PATH "/sys/fs/cgroup/"

class CGroup{
private:
    std::string cgroupPath;

    void addController(const std::string& controller);
public:
    CGroup() = delete;
    CGroup(const std::string& cgroupPath);
    ~CGroup();

    void addProcess(pid_t pid);

    void setCpuLimit(const std::string& maxUsage, const std::string& period);

    void setHardMemoryLimit(const std::string& limit);
    void setThrottlingMemoryLimit(const std::string& limit);
    void setSwapLimit(const std::string& limit);

    void setPidLimit(const size_t& limit);
};




#endif //MINI_DOCKER_CGROUP_H
