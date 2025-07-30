# Dockerfile

# ---- Stage 1: The Builder ----
# We use a Debian-based image with g++, cmake, and git installed
FROM gcc:latest AS builder

# Set the working directory inside the container
WORKDIR /app

# Install necessary dependencies for building our app and fetching Crow
RUN apt-get update && apt-get install -y cmake git libasio-dev libboost-system-dev

# Clone the Crow framework from its GitHub repository
RUN git clone https://github.com/CrowCpp/Crow.git

# Copy our application source code into the container
COPY src/main.cpp .

# Compile the application using g++
# -I Crow/include: Tells g++ to look for header files in the Crow directory
# -o api: Specifies the output executable file name
# -std=c++17: Uses the C++17 standard
# -lpthread -lboost_system: Links necessary libraries that Crow depends on
RUN g++ -I Crow/include -o api main.cpp -lpthread -lboost_system -static-libstdc++

# ---- Stage 2: The Final Image ----
# We use a minimal Debian slim image for a smaller footprint
FROM debian:stable-slim AS final

# Set the working directory
WORKDIR /app

# Install runtime dependencies needed by our compiled app (like Boost)
RUN apt-get update && apt-get install -y libboost-system-dev && rm -rf /var/lib/apt/lists/*

# Copy the compiled executable from the builder stage
COPY --from=builder /app/api .

# Expose port 18080 to the outside world
EXPOSE 18080

# The command to run when the container starts
CMD ["./api"]