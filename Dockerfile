# Dockerfile

# ---- Stage 1: The Builder ----
# We use a Debian-based image with g++, cmake, and git installed
FROM gcc:latest AS builder

# Set the working directory inside the container
WORKDIR /app

# Install necessary dependencies for building our app and fetching Crow
RUN apt-get update && apt-get install -y cmake git libasio-dev libboost-system-dev libhiredis-dev libsqlite3-dev libssl-dev libsodium-dev

# Clone the Crow framework from its GitHub repository
RUN git clone https://github.com/CrowCpp/Crow.git

# Clone the spdlog library
RUN git clone https://github.com/gabime/spdlog.git

# Clone the jwt-cpp library
RUN git clone https://github.com/Thalhammer/jwt-cpp.git

# Clone and build Google Test
RUN git clone https://github.com/google/googletest.git && \
    cd googletest && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Copy our application source code into the container
COPY src/ src/
COPY src/utils.h src/
COPY tests/main_test.cpp tests/

# Compile the application using g++
# -I Crow/include: Tells g++ to look for header files in the Crow directory
# -I spdlog/include: Tells g++ to look for header files in the spdlog directory
# -I jwt-cpp/include: Tells g++ to look for header files in the jwt-cpp directory
# -o api: Specifies the output executable file name
# -std=c++17: Uses the C++17 standard
# -lpthread -lboost_system: Links necessary libraries that Crow depends on
RUN g++ -I /app/Crow/include -I /app/spdlog/include -I /app/jwt-cpp/include -o api src/main.cpp -lpthread -lboost_system -static-libstdc++ -lhiredis -lsqlite3 -lssl -lcrypto -lsodium

# Compile and run the tests
RUN g++ -I /app/Crow/include -I /app/spdlog/include -I /app/jwt-cpp/include -I /usr/local/include -o tests/main_test tests/main_test.cpp -lpthread -lboost_system -static-libstdc++ -lhiredis -lsqlite3 -lssl -lcrypto -lsodium -lgtest -lgtest_main && \
    ./tests/main_test

# ---- Stage 2: The Final Image ----
# We use a minimal Debian slim image for a smaller footprint
FROM debian:stable-slim AS final

# Set the working directory
WORKDIR /app

# Install runtime dependencies needed by our compiled app (like Boost)
RUN apt-get update && apt-get install -y libboost-system-dev libhiredis0.14 libsqlite3-0 libssl-dev libsodium23 && rm -rf /var/lib/apt/lists/*

# Copy the compiled executable from the builder stage
COPY --from=builder /app/api .

# Expose port 18080 to the outside world
EXPOSE 18080

# The command to run when the container starts
CMD ["./api"]