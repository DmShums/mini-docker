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
public:
    CGroup() = delete;
    CGroup(const std::string& cgroupPath);

    void addProcess(pid_t pid);
    void setCpuLimit(const std::string& maxUsage, const std::string& period);
    void setHardMemoryLimit(const std::string& limit);
};



void setThrottlingMemoryLimit(const std::string& cgroupPath, const std::string& limit);
void setSwapLimit(const std::string& cgroupPath, const std::string& limit);
void setPidLimit(const std::string& cgroupPath, const size_t& limit);
void addCgroup(const std::string& cgroupPath);
void addController(const std::string& cgroupPath, const std::string& controller);
void removeController(const std::string& cgroupPath, const std::string& controller);
void removeCgroup(const std::string& cgroupPath);


#endif //MINI_DOCKER_CGROUP_H
