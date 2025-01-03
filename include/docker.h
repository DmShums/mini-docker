#ifndef MINI_DOCKER_DOCKER_H
#define MINI_DOCKER_DOCKER_H

#include <cstdlib>
#include <memory>
#include <vector>
#include <unordered_map>
#include "container.h"
#include <poll.h>

#define NO_LISTENED_CONTAINER (-1)
#define BUF_SIZE 1024

class Docker {
private:
    std::vector<std::unique_ptr<Container>> containers;
    size_t listened_container = NO_LISTENED_CONTAINER;

    std::unordered_map<std::string, size_t> name_to_idx;

    bool isNameUnique(const std::string& name) const {
        return name_to_idx.find(name) == name_to_idx.end();
    }

    bool checkName(const std::string& name) const {
        if (name_to_idx.find(name) == name_to_idx.end()) {
            std::cerr << "Container with name '" << name << "' does not exist." << std::endl;
            return false;
        }
        return true;
    }
    void setnonblocking(int fd);
public:
    void create(const ContainerConfig& cfg);
    void remove(const std::string& name);
    void run(const std::string& name, bool toAttach);
    void kill(const std::string& name);
    void stop(const std::string& name);
    void resume(const std::string& name);
    void attach(const std::string& name, bool sendMsg);
    void list();
    void stop(size_t idx);
    void resume(size_t idx);
};


#endif //MINI_DOCKER_DOCKER_H
