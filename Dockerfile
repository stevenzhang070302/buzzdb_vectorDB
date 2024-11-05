# Dockerfile
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    libeigen3-dev \
    git \
    libjsoncpp-dev \
    && rm -rf /var/lib/apt/lists/*

# Install spdlog
RUN git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j && \
    make install && \
    cd ../../ && \
    rm -rf spdlog

# Install httplib (single header)
RUN mkdir -p /usr/local/include/httplib && \
    wget -O /usr/local/include/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

# Install nlohmann/json (single header)
RUN wget -O /usr/local/include/json.hpp https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Build the project
RUN mkdir build && cd build && cmake .. && make

# Expose port
EXPOSE 8080

# Entry point
CMD ["./build/BuzzDB"]
