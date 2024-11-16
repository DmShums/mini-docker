// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "../include/container.h"

#include <filesystem>

Container::Container(const ContainerConfig& cfg): cfg(cfg), cgroup(cfg.name) {
    if (cfg.mem_hard_limit) {
        cgroup.setHardMemoryLimit(std::to_string(cfg.mem_hard_limit) + "M");
    }

    if (cfg.mem_throttling_limit) {
        cgroup.setThrottlingMemoryLimit(std::to_string(cfg.mem_throttling_limit));
    }

    if (cfg.swap_limit) {
        cgroup.setSwapLimit(std::to_string(cfg.swap_limit) + "M");
    }

    if (cfg.pids_limit != NOT_SET) {
        cgroup.setPidLimit(cfg.pids_limit);
    }
}

int Container::isolate_filesystem() {
    isolated_fs = 1;
    // Create a new mount namespace
    if (unshare(CLONE_NEWNS) == -1) { // further we will add more flags
        std::cerr << "Failed to create a new mount namespace" << std::endl;
        return 1;
    }

    const char* new_root = cfg.new_root.c_str();
    auto put_old_str = cfg.new_root + "/old_root";
    const char* put_old = put_old_str.c_str();

    std::filesystem::create_directories(new_root);
    std::filesystem::create_directories(put_old);

    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1) {
        std::cerr << "mount-MS_PRIVATE" << std::endl;
        return 1;
    }

    if (mount(new_root, new_root, nullptr, MS_BIND | MS_REC, nullptr) == -1) {
        std::cerr << "Failed to bind mount " << new_root << " to itself" << std::endl;
        return 1;
    }

    for (const auto& mntPoint: defaultMountPoints) {
        auto src = mntPoint.substr(0, mntPoint.find(':'));
        auto dst = mntPoint.substr(mntPoint.find(':') + 1);

        if (!std::filesystem::exists(src) || !std::filesystem::is_directory(src)) {
            std::cerr << "Source directory " << src << " does not exist or is not a directory" << std::endl;
            return 1;
        }

        auto full_dst = new_root + dst;
        std::filesystem::create_directories(full_dst);

        if (mount(src.c_str(), full_dst.c_str(), "ext4", MS_BIND | MS_REC | MS_RDONLY, nullptr) == -1) {
            std::cerr << "Failed to bind mount " << src << " to " << full_dst << ": " << strerror(errno) << std::endl;
            return 1;
        }
    }
    
    for (const auto& mntPoint: cfg.mntPoints) {
        auto src = mntPoint.substr(0, mntPoint.find(':'));
        auto dst = mntPoint.substr(mntPoint.find(':') + 1);

        auto full_dst = new_root + dst;
        std::filesystem::create_directories(full_dst);

        if (mount(src.c_str(), full_dst.c_str(), "ext4", MS_BIND | MS_REC, nullptr) == -1) {
            std::cerr << "Failed to bind mount " << src << " to " << full_dst << ": " << strerror(errno) << std::endl;
            return 1;
        }
    }

    for (const auto& copyPoint: cfg.copyPoints) {
        auto src = copyPoint.substr(0, copyPoint.find(':'));
        auto dst = copyPoint.substr(copyPoint.find(':') + 1);

        auto full_dst = new_root + dst;
        std::filesystem::create_directories(full_dst);
        std::filesystem::copy(src, full_dst, std::filesystem::copy_options::recursive);
    }


    if (syscall(SYS_pivot_root, new_root, put_old) == -1) {
        std::cout << "pivot_root " << new_root << " " << put_old << std::endl;
        std::cerr << "Failed to pivot root from " << new_root << " to " << put_old << ": " << strerror(errno) << std::endl;
        return 1;
    }

    if (chdir("/") == -1) {
        std::cerr << "Failed to change working directory to /: " << strerror(errno) << std::endl;
        return 1;
    }

    if (umount2("/old_root", MNT_DETACH) == -1) {
        std::cerr << "Failed to unmount old root directory /old_root: " << strerror(errno) << std::endl;
        return 1;
    }

    if (rmdir("/old_root") == -1) {
        std::cerr << "Failed to remove old root directory /old_root: " << strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}

void Container::run() {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        return;
    }

    if (pid == 0) {
        if (isolate_filesystem() != 0) {
            std::cerr << "Failed to isolate filesystem" << std::endl;
            clear_filesystem();
            return;
        }

        std::vector<char*> args;
        for (const auto& arg: cfg.argv) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }

        args.push_back(nullptr);

        if (chmod(args[0], 0777) == -1) {
            std::cerr << "Failed to change permissions of args[0]: " << strerror(errno) << std::endl;
            return;
        }

        if (access(args[0], X_OK) != 0) {
            std::cerr << "No access to execute " << args[0] << ": " << strerror(errno) << std::endl;
            return;
        }

        if (execv(args[0], args.data()) == -1) {
            std::cerr << "Failed to execute " << args[0] << ": " << strerror(errno) << std::endl;
            std::cerr << "errno: " << errno << std::endl;
            std::cerr << "Full command: ";
            for (const auto& arg: args) {
                std::cerr << arg << " ";
            }
            std::cerr << std::endl;
            return;
        }

        clear_filesystem();
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void Container::clear_filesystem() {
    isolated_fs = 0;

    if (umount2(cfg.new_root.c_str(), MNT_DETACH) == -1) {
        std::cerr << "Failed to unmount " << cfg.new_root << ": " << strerror(errno) << std::endl;
    }

    for (const auto& mntPoint: cfg.mntPoints) {
        auto dst = mntPoint.substr(mntPoint.find(':') + 1);
        auto full_dst = cfg.new_root + dst;

        if (umount2(full_dst.c_str(), MNT_DETACH) == -1) {
            std::cerr << "Failed to unmount " << full_dst << ": " << strerror(errno) << std::endl;
        }
    }

    for (const auto& mntPoint: defaultMountPoints) {
        auto dst = mntPoint.substr(mntPoint.find(':') + 1);
        auto full_dst = cfg.new_root + dst;

        if (umount2(full_dst.c_str(), MNT_DETACH) == -1) {
            std::cerr << "Failed to unmount " << full_dst << ": " << strerror(errno) << std::endl;
        }
    }

    // remove new_root
    if (std::filesystem::exists(cfg.new_root)) {
        std::filesystem::remove_all(cfg.new_root);
    }
}


Container::~Container() {
    if (isolated_fs) {
        clear_filesystem();
    }
}