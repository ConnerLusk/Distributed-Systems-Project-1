#include "helpers.hpp"
#include <fstream>

void uploadHosts(char* currentContainer, std::vector<std::string>* allContainers) {
    std::ifstream inFile;
    std::string input;
    inFile.open("hostfile.txt");

    if (!inFile) {
        fprintf(stderr, "Failed to load hostnames!\n");
        exit(EXIT_FAILURE);
    }
    
    while (inFile >> input) {
        if (input != currentContainer) {
            allContainers->push_back(input);
        }
    }
}

bool includesHost(char* currentContainer, std::vector<std::string>* allContainers) {
    for (int i = 0; i < allContainers->size(); i++) {
        if (allContainers->at(i) == currentContainer) {
            return true;
        }
    }
    return false;
}