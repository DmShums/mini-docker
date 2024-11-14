#ifndef MINI_DOCKER_CONTAINERCONFIG_H
#define MINI_DOCKER_CONTAINERCONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

#define NOT_SET -1

class ContainerConfig{
private:
    const std::unordered_map<std::string, int&> optionalIntArguments = {
            {"mem_hard_limit", mem_hard_limit},
            {"mem_throttling_limit", mem_throttling_limit},
            {"swap_limit", swap_limit},
            {"pids_limit", pids_limit}
    };


public:
    std::string name;
    std::vector<std::string> argv;
    std::vector<std::string> mntPoints;
    int mem_hard_limit, mem_throttling_limit, swap_limit, pids_limit;

    ContainerConfig() = delete;
    explicit ContainerConfig(std::string path){
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(path, pt);

        // parse argv
        argv.emplace_back(pt.get<std::string>("bin"));
        for (const auto &item: pt.get_child("args")) {
            argv.push_back(item.second.data());
        }

        // parse name if exists else name is bin name
        auto name_res = pt.get_optional<std::string>("name");
        if (name_res) {
            name = name_res.value();
        } else {
            name = argv[0];
        }

        // parse memory limitations
        for (const auto& [argName, varRef] : optionalIntArguments) {
            auto res = pt.get_optional<int>(argName);
            varRef = res ? res.value() : NOT_SET;
        }

        // parse mount points
        for (const auto &item: pt.get_child("mount_points")) {
            mntPoints.push_back(item.second.data());
        }
    }
};

#endif //MINI_DOCKER_CONTAINERCONFIG_H
