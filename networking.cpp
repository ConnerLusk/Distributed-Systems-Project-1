#include "networking.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iomanip>
#include <vector>
#include <string>
#include <set>
#include <string>

#include "helpers.hpp"

#define PORT "3000"
#define MAXBUFFER 100

int startListening(std::vector<std::string>* hostList, char* currentContainer) {
    int fd, rv; // file descriptor and return value
    struct addrinfo hints, *result, *res; // address info
    struct sockaddr incomingAddress; // incoming address
    socklen_t addressLength = sizeof incomingAddress; // size of the incoming address
    char buffer[MAXBUFFER]; // buffer to hold the return value
    std::set<std::string> seen; // holds all of the containers that have been seen


    // Initialize hints address info fields
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4 Adress
    hints.ai_socktype = SOCK_DGRAM; // UDP Schemea
    hints.ai_flags = AI_PASSIVE; // This will allow the socket to be binded

    // Gets the address information and exits on failure
    rv = getaddrinfo(NULL, PORT, &hints, &result);
    if (rv != 0) {
        fprintf(stderr, "Failed to get address information!\n");
        return -1;
    }

    // create socket and bind to the first
    for(res = result; res != NULL; res = res->ai_next) {
        fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1) continue;
        rv = bind(fd, res->ai_addr, res->ai_addrlen);
        if (rv == -1) continue;
        break;
    }

    // If unable to bind exit with an error
    if (res == NULL) {
        fprintf(stderr, "Failed to bind to a socket!\n");
        return -1;
    }

    // Don't need anymore so free
    freeaddrinfo(result);

    while (true) {
        int recieve = recvfrom(fd, buffer, MAXBUFFER-1 , 0, &incomingAddress, &addressLength);
        bool hostIncluded = includesHost(buffer, hostList);
        if (hostIncluded) {
            buffer[recieve] = '\0';
            seen.insert(buffer);
        }
        if (seen.size() == hostList->size()) {
            fprintf(stderr, "%s %s\n", currentContainer, "ready");
            break;
        }
    }
    close(fd);
    return 0;
}

int sendMessage(char* ipAddress, char* message) {
    // Initialize Data
    int fd, rv;
    struct addrinfo hints, *result, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // Gets the address information and exits on failure
    rv = getaddrinfo(ipAddress, PORT, &hints, &result);
    if (rv != 0) {
        // fprintf(stderr, "Failed to get address information!\n");
        return -1;
    }

    // Create socket
    for(res = result; res != NULL; res = res->ai_next) {
        fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1) continue;
        break;
    }

    // If socket does not exist exits on failure
    if (res == NULL) {
        // fprintf(stderr, "Failed to create socket!\n");
        return -1;
    }

    // Sends the message to the port
    rv = sendto(fd, message, 10, MSG_DONTWAIT, res->ai_addr, res->ai_addrlen);
    if (rv == -1) {
        // fprintf(stderr, "Failed to send data!\n");
        return -1;
    }
    
    // Close the programs
    freeaddrinfo(result);
    close(fd);
    return 0;
}

void sendMessages(std::vector<std::string>* hostList, char* currentContainer) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < hostList->size(); i++) {
            std::string currentHost = hostList->at(i);
            const int length = currentHost.length();
            char* host = new char[length + 1];
            strcpy(host, currentHost.c_str());
            int retVal = sendMessage(host, currentContainer);
            if (retVal == -1) {
                fprintf(stderr, "%s %s %s %s\n","failed sending from", currentContainer, "to", host);
            }
        }
        sleep(1);
    }
}