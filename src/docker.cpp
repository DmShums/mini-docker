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

void Docker::run(const std::string& name, bool toAttach) {
    if (!checkName(name)) return;

    containers[name_to_idx[name]]->run(toAttach);

    if (toAttach) {
        attach(name, toAttach);
    }
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

void Docker::attach(const std::string& name, bool sendMsg){
    size_t idx = name_to_idx[name];

    if (containers[idx]->pipeFromProc == PIPE_NOT_SET ||
        containers[idx]->pipeToProc   == PIPE_NOT_SET ||
        containers[idx]->procPid      == PID_NOT_SET){
        std::cerr << "Cannot attach to not running process" << std::endl;
        return;
    }

    // setup poll
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = containers[idx]->pipeFromProc;
    fds[1].events = POLLIN;

    char buffer[BUF_SIZE];

    if (sendMsg) {
        write(containers[idx]->pipeToProc, ATTACHED_MSG, sizeof(ATTACHED_MSG));
    }

    // be transparent until "detach" appears in stdin message
    while (true){
        if (poll(fds, 2, -1) == -1){
            std::cerr << "Failed to create poll" << std::endl;
            return;
        }

        if (fds[0].revents & POLLIN) {
            ssize_t bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE - 1);
            if (bytesRead > 0){
                buffer[bytesRead] = '\0';
                std::string input(buffer);

                if (input.find("detach") != std::string::npos) {
                    break;
                } else {
                    write(containers[idx]->pipeToProc, buffer, bytesRead);
                }
            } else if (bytesRead == 0){
                break;
            } else {
                std::cerr << "Failed to read from STDIN" << std::endl;
            }
        }

        if (fds[1].revents & POLLIN) {
            ssize_t bytesRead = read(containers[idx]->pipeFromProc, buffer, BUF_SIZE - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                std::cout << buffer;
                std::cout.flush();
            } else if (bytesRead == 0) {
                // pipe closed because the proc ended;
                std::cerr << "Pipe from the process has been closed." << std::endl;
                containers[idx]->pipeFromProc = PIPE_NOT_SET;
                containers[idx]->pipeToProc = PIPE_NOT_SET;
                break;
            } else {
                std::cerr << "Failed to read from process pipe: " << strerror(errno) << std::endl;
            }
        }

        if (fds[1].revents & POLLHUP) {
            // The pipeFromProc file descriptor has been closed by the remote end
            std::cerr << "Pipe to the process has been closed." << std::endl;
            break;
        }

        if (fds[1].revents & POLLERR) {
            // An error occurred on the file descriptor
            std::cerr << "An error occurred on the pipe to the process." << std::endl;
            break;
        }
    }
}

void Docker::setnonblocking(int fd) {
    int opts;

    opts = fcntl(fd,F_GETFL);
    if (opts < 0) {
        perror("Couldn't get file descriptor flags");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(fd,F_SETFL,opts) < 0) {
        perror("Couldn't set file descriptor to non-blocking");
        exit(EXIT_FAILURE);
    }
}