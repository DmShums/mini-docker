// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "namespaces.hpp"

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


int main() {
    pid_t child_pid = createPidNamespace(childFunction, nullptr);
    pid_t uts_pid = createPidNamespace(utsChildFunction, nullptr);

    waitForChildProcess(child_pid);
    waitForChildProcess(uts_pid);

    return 0;
}