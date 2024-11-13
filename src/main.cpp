// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#include "cgroups.hpp"

int main(){
    std::string cgroupPath = "myCgroup";
    addCgroup("myCgroup");
    std::cout << "Cgroup created" << std::endl;

    addProcessToCgroup(cgroupPath, getpid());

    setMemoryLimit(cgroupPath, "10M");
    setCpuLimit(cgroupPath, "50000", "100000");
    setPidLimit(cgroupPath, 3);

    std::vector<pid_t> pids;

    for (size_t i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            std::cout << "created process with PID: " << getpid() << std::endl;
            sleep(10);
            exit(0);
        } else if (pid > 0){
            pids.push_back(pid);
        } else {
            std::cerr << "failed to fork" << std::endl;
        }
    }

    for (auto pid : pids){
        waitpid(pid, nullptr, 0);
    }
    std::cout << "Process finished." << std::endl;
//    removeCgroup(cgroupPath);

    return 0;
}