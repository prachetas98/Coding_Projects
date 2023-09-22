# Use Ubuntu as the base image
FROM ubuntu:20.04

# Update package lists and install necessary packages
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy your C++ source code into the container (assuming it's in the same directory as the Dockerfile)
COPY . /app

# Compile and run the C++ program
CMD ["g++", "main.cc", "-o", "main", "&&", "./main"]
