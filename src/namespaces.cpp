#include "namespaces.hpp"

// Define the stack for the cloned child process
char child_stack[STACK_SIZE];

pid_t createPidNamespace(int (*child_func)(void*), void* arg) {
    pid_t child_pid = clone(child_func, child_stack + STACK_SIZE, SIGCHLD | CLONE_NEWPID, arg);

    if (child_pid == -1) {
        perror("clone failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "PID Namespace created. Child PID: " << child_pid << std::endl;
    return child_pid;
}

void waitForChildProcess(pid_t child_pid) {
    if (child_pid != -1) {
        waitpid(child_pid, nullptr, 0);
        std::cout << "Child process terminated." << std::endl;
    } else {
        std::cerr << "Error: No child process to wait for." << std::endl;
    }
}