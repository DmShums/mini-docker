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
        std::cerr << "Failed to open cgroup.procs. Check cgroup name" << std::endl;
        return;
    }
    procsFile << pid;
    procsFile.close();
}

void addController(const std::string& cgroupName, const std::string& controller){
//    adds controller to cgroup
    std::ofstream controllersFile(CGROUP_PATH + cgroupName + "/cgroup.subtree_controllers");
    if (!controllersFile) {
        std::cerr << "Failed to open cgroup.subtree_controllers. Check cgroup name" << std::endl;
        return;
    }
    controllersFile << ("+" + controller);
    controllersFile.close();
}

void removeController(const std::string& cgroupName, const std::string& controller){
//    removes controller to cgroup
    std::ofstream controllersFile(CGROUP_PATH + cgroupName + "/cgroup.subtree_controllers");
    if (!controllersFile) {
        std::cerr << "Failed to open cgroup.subtree_controllers. Check cgroup name" << std::endl;
        return;
    }
    controllersFile << ("-" + controller);
    controllersFile.close();
}

void setHardMemoryLimit(const std::string& cgroupName, const std::string& limit) {
//    adds RAM usage limit to cgroup with cgroupPath after which the process will be killed
    std::ofstream memLimitFile(CGROUP_PATH + cgroupName + "/memory.max");
    if (!memLimitFile) {
        std::cerr << "Failed to open memory.max. Check if memory added to controllers" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void setSwapLimit(const std::string& cgroupName, const std::string& limit){
//    adds Swap usage limit to cgroup with cgroupPath
    std::ofstream memLimitFile(CGROUP_PATH + cgroupName + "/memory.swap.max");
    if (!memLimitFile) {
        std::cerr << "Failed to open memory.max. Check if memory added to controllers" << std::endl;
        return;
    }
    memLimitFile << limit;
    memLimitFile.close();
}

void setCPUWeight(const std::string& cgroupName, uint weight){
//    sets weight of the cGroup for CPU resources
    if (weight > 1000 || weight < 1){
        std::cerr << "Wrong weight value. The weight should be in range of [1;1000]" << std::endl;
        return;
    }

    std::ofstream weightFile(CGROUP_PATH + cgroupName + "/cpu.weight");
    if (!weightFile) {
        std::cerr << "Failed to open cpu.weight. Check if cpu added to controllers" << std::endl;
    }

    weightFile << weight;
    weightFile.close();
}

void setCpuLimit(const std::string& cgroupName, const std::string& maxUsage, const std::string& period) {
//    adds CPU time limitation timer resets after each period
    std::ofstream cpuLimitFile(CGROUP_PATH + cgroupName +  "/cpu.max");
    if (!cpuLimitFile) {
        std::cerr << "Failed to open cpu.max. Check if cpu added to controllers" << std::endl;
        return;
    }
    cpuLimitFile << maxUsage << " " << period;
    cpuLimitFile.close();
}

void setPidLimit(const std::string& cgroupName, const size_t& limit){
    std::ofstream  pidLimitFile(CGROUP_PATH + cgroupName + "/pids.max");
    if (!pidLimitFile) {
        std::cerr << "Failed to open pids.max. Check if pids added to controllers" << std::endl;
        return;
    }

    pidLimitFile << limit;
    pidLimitFile.close();
}