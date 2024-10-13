// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MINI_DOCKER_PID_NAMESPACE_HPP
#define MINI_DOCKER_PID_NAMESPACE_HPP

#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

#define STACK_SIZE (1024 * 1024)

///////// PID namespace /////////

pid_t createPidNamespace(int (*child_func)(void*), void* arg = nullptr);

// Function to wait for the child process to terminate
void waitForChildProcess(pid_t child_pid);

///////// UTS namespace /////////

pid_t createUtsNamespace(int (*child_func)(void*), void* arg);



#endif //MINI_DOCKER_PID_NAMESPACE_HPP