// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "cgroups.hpp"

void addCgroup(const std::string& cgroupName) {
    std::string newCgroupPath = CGROUP_PATH + cgroupName;

    if (mkdir(newCgroupPath.c_str(), 0755) != 0) {
        std::cerr << "Failed to create cgroup "<< cgroupName << std::endl;
    }
}

void removeCgroup(const std::string& cgroupName) {
    if (rmdir((CGROUP_PATH + cgroupName).c_str()) != 0) {
        std::cerr << "Failed to remove cgroup "<< cgroupName << std::endl;
    }
}

void addProcessToCgroup(const std::string& cgroupName, pid_t pid) {
//    creates new process
    std::ofstream procsFile(CGROUP_PATH + cgroupName + "/cgroup.procs");
    if (!procsFile) {
        std::cerr << "Failed to add process to cgroup" << std::endl;
        return;
    }
    procsFile << pid;
    procsFile.close();
}

void setMemoryLimit(const std::string& cgroupName, const std::string& limit) {
//    adds RAM usage limit to cgroup with cgroupPath
    std::ofstream memLimitFile(CGROUP_PATH + cgroupName + "/memory.max");
    if (!memLimitFile) {
        std::cerr << "Failed to set memory limit" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void setCpuLimit(const std::string& cgroupName, const std::string& maxUsage, const std::string& period) {
//    adds CPU time limitation timer resets after each perid
    std::ofstream cpuLimitFile(CGROUP_PATH + cgroupName +  "/cpu.max");
    if (!cpuLimitFile) {
        std::cerr << "Failed to set CPU limit" << std::endl;
        return;
    }
    cpuLimitFile << maxUsage << " " << period;
    cpuLimitFile.close();
}

void setPidLimit(const std::string& cgroupName, const size_t& limit){
    std::ofstream  pidLimitFile(CGROUP_PATH + cgroupName + "/pids.max");
    if (!pidLimitFile) {
        std::cerr << "Failed to set PIDs limit" << std::endl;
        return;
    }

    pidLimitFile << limit;
    pidLimitFile.close();
}