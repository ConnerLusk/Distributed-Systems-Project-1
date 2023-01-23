#include <vector>
#include <thread>

#include "helpers.hpp"
#include "networking.hpp"

int main(int argc, char *argv[]) {
    char* containerName = argv[1];
    std::vector<std::string>* containers = new std::vector<std::string>;
    uploadHosts(containerName, containers);

    std::thread t1(startListening, containers, containerName);
    std::thread t2(sendMessages, containers, containerName);

    t1.join();
    t2.join();
}