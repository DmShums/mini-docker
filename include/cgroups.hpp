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
void setMemoryLimit(const std::string& cgroupPath, const std::string& limit);
void addCgroup(const std::string& cgroupName);
void removeCgroup(const std::string& cgroupName);

#endif //MINI_DOCKER_CGROUPS_HPP
