#include <iostream>
#include <filesystem>
#include <fstream>

int main() {
    // make file test.txt
    const char* path = "/test.txt";

    std::ofstream file(path);
    file << "Hello, World!";
    file.close();

    std::cout << realpath(path, nullptr) << std::endl;
    std::cout << "Hello, World from test2!" << std::endl;

    return 0;
}