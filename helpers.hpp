#ifndef HELPERS_HPP_
#define HELPERS_HPP_
#include <vector>
#include <string>

void uploadHosts(char* currentContainer, std::vector<std::string>* allContainers);
bool includesHost(char* currentContainer, std::vector<std::string>* allContainers);

#endif