// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <unistd.h>
#include <sys/wait.h>

#include "cgroups.hpp"

int main(){
    std::string cgroupPath = "myCgroup";
    addCgroup("myCgroup");
    std::cout << "Cgroup created" << std::endl;

    setMemoryLimit(cgroupPath, "10M");
    setCpuLimit(cgroupPath, "50000", "100000");
    setPidLimit(cgroupPath, 3);

    pid_t pid = fork();
    if (pid == 0) {
        execlp("sh", "sh", nullptr);

    } else {
        addProcessToCgroup(cgroupPath, pid);

        waitpid(pid, nullptr, 0);
        std::cout << "Process finished." << std::endl;

        removeCgroup(cgroupPath);
    }

    return 0;
}