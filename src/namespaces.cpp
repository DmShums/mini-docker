#include "namespaces.hpp"

// Define the stack for the cloned child process
char child_stack[STACK_SIZE];

// Child function for PID namespace
int childFunction(void* arg) {
    std::cout << "Inside the new PID namespace!" << std::endl;
    
    // Mount /proc to view the processes inside the namespace
    if (mount("proc", "/proc", "proc", 0, nullptr) == -1) {
        perror("mount failed");
        return -1;
    }

    // Show the processes in the namespace
    system("ps aux");

    // Keep the child process alive to observe the namespace
    system("sleep 10");

    return 0;
}

// Child function for UTS namespace
int utsChildFunction(void* arg) {
    std::cout << "Inside the new UTS namespace!" << std::endl;

    // Set the hostname and domain name for the UTS namespace
    if (sethostname("myhostname", strlen("myhostname")) == -1) {
        perror("sethostname failed");
        return -1;
    }

    if (setdomainname("mydomain", strlen("mydomain")) == -1) {
        perror("setdomainname failed");
        return -1;
    }

    // Display the hostname and domain name
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    std::cout << "Hostname: " << hostname << std::endl;

    char domainname[256];
    getdomainname(domainname, sizeof(domainname));
    std::cout << "Domainname: " << domainname << std::endl;

    // Keep the child process alive to observe the namespace
    system("sleep 10");

    return 0;
}

pid_t createPidNamespace(int (*child_func)(void*), void* arg) {
    pid_t child_pid = clone(child_func, child_stack + STACK_SIZE, SIGCHLD | CLONE_NEWPID | CLONE_NEWUTS, arg);

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