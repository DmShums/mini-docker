// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "docker.h"

void Docker::create(const ContainerConfig& cfg) {
    if (!isNameUnique(cfg.name)) {
        std::cerr << "Failed to create container. Name '" << cfg.name << "' is already in use." << std::endl;
        return;
    }

    containers.push_back(std::make_unique<Container>(cfg));
    name_to_idx[cfg.name] = containers.size() - 1;
}

void Docker::remove(const std::string& name) {
    if (!checkName(name)) return;

    size_t idx = name_to_idx[name];
    containers.erase(containers.begin() + idx);
    name_to_idx.erase(name);

    // Update the indices in the map
    for (auto& pair : name_to_idx) {
        if (pair.second > idx) {
            pair.second--;
        }
    }
}

void Docker::run(const std::string& name) {
    if (!checkName(name)) return;

    containers[name_to_idx[name]]->run();
}

void Docker::kill(const std::string& name) {
    if (!checkName(name)) return;

    size_t idx = name_to_idx[name];
    ::kill(containers[idx]->procPid, SIGKILL);
    containers[idx]->procPid = PID_NOT_SET;
}

void Docker::stop(const std::string& name) {
    if (!checkName(name)) return;

    ::kill(containers[name_to_idx[name]]->procPid, SIGSTOP);
}

void Docker::resume(const std::string& name) {
    if (!checkName(name)) return;

    ::kill(containers[name_to_idx[name]]->procPid, SIGCONT);
}

void Docker::list() {
    for (const auto& pair : name_to_idx) {
        size_t idx = pair.second;
        std::cout << "\tname:\t" << pair.first << std::endl
                  << "\tidx:\t" << idx << std::endl;
        std::cout << containers[idx]->info();
    }
}