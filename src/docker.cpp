// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "docker.h"

bool Docker::checkIdx(size_t idx){
    if (containers.size() <= idx){
        std::cerr << "Failed to get container with " << idx << " largest is " << containers.size() - 1 << std::endl;
        return false;
    }
    return true;
}

void Docker::create(const ContainerConfig& cfg){
    containers.push_back(std::make_unique<Container>(cfg));
}

void Docker::remove(size_t idx){
    if (!checkIdx(idx)) return;

    containers.erase(containers.begin() + idx);
}

void Docker::run(size_t idx){
    if (!checkIdx(idx)) return;

    containers[idx]->run();
}

void Docker::kill(size_t idx){
    if (!checkIdx(idx)) return;

    ::kill(containers[idx]->procPid, SIGKILL);
    containers[idx]->procPid = PID_NOT_SET;
}

void Docker::stop(size_t idx){
    if (!checkIdx(idx)) return;

    ::kill(containers[idx]->procPid, SIGSTOP);
}
void Docker::resume(size_t idx){
    if (!checkIdx(idx)) return;

    ::kill(containers[idx]->procPid, SIGCONT);
}

void Docker::list(){
    for (size_t idx = 0; idx < containers.size(); idx++){
        std::cout << "\tname:\t" << containers[idx]->getName() << std::endl
            << "\tidx:\t" << idx << std::endl;
        std::cout << containers[idx]->info();
    }
}

//void Docker::listen(size_t idx);
//void Docker::detach();