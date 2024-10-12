// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <unistd.h>
#include <sys/wait.h>

#include "cgroups.hpp"

unsigned long long getTotalSystemMemory()
{
    long pages = sysconf(_SC_AVPHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size / (1024 * 1024);
}

int main(){
    std::string cgroupPath = "myCgroup";
    addCgroup("myCgroup");
    std::cout << "Cgroup created" << std::endl;

    setMemoryLimit(cgroupPath, "10M");
    setCpuLimit(cgroupPath, "50000", "100000");

    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "Hello restricted world!" << std::endl;

//        don't know why but shows to much memory(
//        made process wait for input to be able to check manually if it appears in the /sys/fs/cgroup/myCgroup/cgroup.procs
//        std::cout << "RAM:\t" << std::to_string(getTotalSystemMemory()) << "MB" << std::endl;
        std::cin.ignore();
    } else {
        addProcessToCgroup(cgroupPath, pid);

        waitpid(pid, nullptr, 0);
        std::cout << "Process finished." << std::endl;

        removeCgroup(cgroupPath);
    }

    return 0;
}