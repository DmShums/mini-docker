// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "namespaces.hpp"

int main() {
    pid_t child_pid = createPidNamespace(childFunction, nullptr);
    pid_t uts_pid = createPidNamespace(utsChildFunction, nullptr);

    waitForChildProcess(child_pid);
    waitForChildProcess(uts_pid);

    return 0;
}