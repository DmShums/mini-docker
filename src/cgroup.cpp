// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "cgroup.h"

CGroup::CGroup(const std::string& cgroupPath) {
//    creates new CGroup with name cgroupPath and adds itself to the group
    std::string newCgroupPath = CGROUP_PATH + cgroupPath;

    if (mkdir(newCgroupPath.c_str(), 0755) != 0) {
        std::cerr << "Failed to create cgroup "<< cgroupPath << std::endl;
    }

    addController("pid");
    addController("memory");
    addController("cpu");

    addProcess(getpid());
}

void CGroup::addController(const std::string& controller){
//    adds controller to cgroup
    std::ofstream controllersFile(CGROUP_PATH + cgroupPath + "/cgroup.subtree_controllers");
    if (!controllersFile) {
        std::cerr << "Failed to open cgroup.subtree_controllers. Check cgroup name" << std::endl;
        return;
    }
    controllersFile << ("+" + controller);
    controllersFile.close();
}

void CGroup::addProcess(pid_t pid) {
//    creates new process
    std::ofstream procsFile(CGROUP_PATH + cgroupPath + "/cgroup.procs");
    if (!procsFile) {
        std::cerr << "Failed to open cgroup.procs. Check cgroup name" << std::endl;
        return;
    }
    procsFile << pid;
    procsFile.close();
}

void CGroup::setCpuLimit(const std::string& maxUsage, const std::string& period) {
//    adds CPU time limitation timer resets after each period
    std::ofstream cpuLimitFile(CGROUP_PATH + cgroupPath +  "/cpu.max");
    if (!cpuLimitFile) {
        std::cerr << "Failed to open cpu.max. Check if cpu added to controllers" << std::endl;
        return;
    }
    cpuLimitFile << maxUsage << " " << period;
    cpuLimitFile.close();
}

void CGroup::setHardMemoryLimit(const std::string& limit) {
//    adds RAM usage limit to cgroup with cgroupPath after which the process will be killed
    std::ofstream memLimitFile(CGROUP_PATH + cgroupPath + "/memory.max");
    if (!memLimitFile) {
        std::cerr << "Failed to open memory.max. Check if memory added to controllers" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void CGroup::setThrottlingMemoryLimit(const std::string &limit) {
//    adds RAM usage limit to cgroup with cgroupPath after which the process will throttle
    std::ofstream memLimitFile(CGROUP_PATH + cgroupPath + "/memory.high");
    if (!memLimitFile) {
        std::cerr << "Failed to open memory.max. Check if memory added to controllers" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void CGroup::setSwapLimit(const std::string& limit){
//    adds Swap usage limit to cgroup with cgroupPath
    std::ofstream memLimitFile(CGROUP_PATH + cgroupPath + "/memory.swap.max");
    if (!memLimitFile) {
        std::cerr << "Failed to open memory.max. Check if memory added to controllers" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void CGroup::setPidLimit(const size_t& limit){
    std::ofstream  pidLimitFile(CGROUP_PATH + cgroupPath + "/pids.max");
    if (!pidLimitFile) {
        std::cerr << "Failed to open pids.max. Check if pids added to controllers" << std::endl;
        return;
    }

    pidLimitFile << limit;
    pidLimitFile.close();
}
