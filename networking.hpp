#ifndef NETWORKING_HPP_
#define NETWORKING_HPP_

#include <vector>
#include <string>

int startListening(std::vector<std::string>* hostList, char* currentContainer);
void sendMessages(std::vector<std::string>* hostList, char* currentContainer);

#endif