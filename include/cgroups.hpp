// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MINI_DOCKER_CGROUPS_HPP
#define MINI_DOCKER_CGROUPS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#define CGROUP_PATH "/sys/fs/cgroup/"

void addProcessToCgroup(const std::string& cgroupPath, pid_t pid);
void setCpuLimit(const std::string& cgroupPath, const std::string& maxUsage, const std::string& period);
void setHardMemoryLimit(const std::string& cgroupPath, const std::string& limit);
void setThrottlingMemoryLimit(const std::string& cgroupPath, const std::string& limit);
void setSwapLimit(const std::string& cgroupPath, const std::string& limit);
void setPidLimit(const std::string& cgroupPath, const size_t& limit);
void addCgroup(const std::string& cgroupPath);
void addController(const std::string& cgroupPath, const std::string& controller);
void removeController(const std::string& cgroupPath, const std::string& controller);
void removeCgroup(const std::string& cgroupPath);

#endif //MINI_DOCKER_CGROUPS_HPP
