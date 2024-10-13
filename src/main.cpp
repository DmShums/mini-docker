// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "namespaces.hpp"


int childFunction(void* arg) {
    std::cout << "Inside the new PID namespace!" << std::endl;
    
    // Mount /proc to view the processes inside the namespace
    if (system("mount -t proc proc /proc") == -1) {
        perror("mount failed");
        return -1;
    }

    // Show the processes in the namespace
    system("ps aux");

    // Keep the child process alive to observe the namespace
    system("sleep 10");

    return 0;
}

int main() {
    pid_t child_pid = createPidNamespace(childFunction);
    waitForChildProcess(child_pid);

    return 0;
}