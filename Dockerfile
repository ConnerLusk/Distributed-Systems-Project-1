# Sets the image to the latest version of ubuntu
FROM ubuntu:latest

ARG containerName


# Update the Ubuntu package repository and install compiler
RUN apt-get update
RUN apt-get install -y g++

# Copy main.cpp from local filesystem to image filesystem
ADD * /app/
WORKDIR /app/
RUN g++ -std=c++0x main.cpp helpers.cpp networking.cpp -o node

# Onces the container opens this runs
ENTRYPOINT ["/app/node"]