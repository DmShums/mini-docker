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

void Docker::run(size_t idx, bool toAttach){
    if (!checkIdx(idx)) return;

    containers[idx]->run(toAttach);

    if(toAttach) {
        attach(idx, toAttach);
    }
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

void Docker::attach(size_t idx, bool sendMsg){
    Container cnt = *containers[idx];

    if (cnt.pipeFromProc == PIPE_NOT_SET || cnt.pipeToProc == PIPE_NOT_SET || cnt.procPid == PID_NOT_SET){
        std::cerr << "Cannot attach to not running process" << std::endl;
        return;
    }

    // setup poll
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = cnt.pipeFromProc;
    fds[1].events = POLLIN;

    char buffer[BUF_SIZE];

    if (sendMsg) {
        write(cnt.pipeToProc, ATTACHED_MSG, sizeof(ATTACHED_MSG));
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
                    write(cnt.pipeToProc, buffer, bytesRead);
                }
            } else if (bytesRead == 0){
                break;
            } else {
                std::cerr << "Failed to read from STDIN" << std::endl;
            }
        }

        if (fds[1].revents & POLLIN) {
            ssize_t bytesRead = read(cnt.pipeFromProc, buffer, BUF_SIZE - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                std::cout << buffer;
                std::cout.flush();
            } else if (bytesRead == 0) {
                // pipe closed because the proc ended;
                std::cerr << "Pipe to the process has been closed." << std::endl;
                break;
            } else {
                perror("read");
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