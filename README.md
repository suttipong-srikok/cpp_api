# C++ REST API with Crow, SQLite, Redis, spdlog, and JWT

This is a simple C++ REST API built using the [Crow](https://github.com/CrowCpp/Crow) micro-framework, backed by a SQLite database for persistent storage, Redis for caching and message queuing, [spdlog](https://github.com/gabime/spdlog) for structured logging, and [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) for JWT-based authentication. The application is containerized using Docker and provides full CRUD (Create, Read, Update, Delete) functionality for managing users.

## Getting Started

### Prerequisites

*   [Docker](https://docs.docker.com/get-docker/)
*   [Docker Compose](https://docs.docker.com/compose/install/)

### Building and Running the Application

1.  **Clone the repository:**

    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```

2.  **Build and run with Docker Compose:**

    ```bash
    docker-compose up --build
    ```

    This command will build the Docker images and start the containers for the C++ application and the Redis server. The API will be available at `http://localhost:8081`.

## API Endpoints

### Authentication

*   **`POST /register`**: Register a new user.

    **Request Body:**

    ```json
    {
        "name": "John Doe",
        "email": "john.doe@example.com",
        "password": "your-password"
    }
    ```

    **Example using `curl`:**

    ```bash
    curl -X POST -H "Content-Type: application/json" -d '{"name":"John Doe","email":"john.doe@example.com","password":"your-password"}' http://localhost:8081/register
    ```

*   **`POST /login`**: Login as an existing user.

    **Request Body:**

    ```json
    {
        "email": "john.doe@example.com",
        "password": "your-password"
    }
    ```

    **Example using `curl`:**

    ```bash
    curl -X POST -H "Content-Type: application/json" -d '{"email":"john.doe@example.com","password":"your-password"}' http://localhost:8081/login
    ```

### Users (Protected)

*   **`GET /users`**: Get a list of all users.

    **Example using `curl`:**

    ```bash
    curl -H "Authorization: Bearer <your-jwt>" http://localhost:8081/users
    ```

*   **`GET /users/<id>`**: Get a specific user by their ID.

    **Example using `curl`:**

    ```bash
    curl -H "Authorization: Bearer <your-jwt>" http://localhost:8081/users/1
    ```

*   **`PUT /users/<id>`**: Update a user's information.

    **Request Body:**

    ```json
    {
        "name": "Jane Doe",
        "email": "jane.doe@example.com"
    }
    ```

    **Example using `curl`:**

    ```bash
    curl -X PUT -H "Content-Type: application/json" -H "Authorization: Bearer <your-jwt>" -d '{"name":"Jane Doe","email":"jane.doe@example.com"}' http://localhost:8081/users/1
    ```

*   **`DELETE /users/<id>`**: Delete a user.

    **Example using `curl`:**

    ```bash
    curl -X DELETE -H "Authorization: Bearer <your-jwt>" http://localhost:8081/users/1
    ```

### Message Queue

*   **`POST /messages`**: Add a message to the queue.

    **Request Body:**

    ```json
    {
        "message": "Your message here"
    }
    ```

    **Example using `curl`:**

    ```bash
    curl -X POST -H "Content-Type: application/json" -d '{"message":"hello world"}' http://localhost:8081/messages
    ```

*   **`GET /messages`**: Retrieve a message from the queue.

    **Example using `curl`:**

    ```bash
    curl http://localhost:8081/messages
    ```

## Project Structure

```
.
├── docker-compose.yml  # Defines the services, networks, and volumes for Docker
├── Dockerfile          # Contains instructions to build the Docker image
├── .gitignore          # Specifies intentionally untracked files to ignore
├── README.md           # This file
└── src/
    └── main.cpp        # The main application source code
```

## Docker

The `Dockerfile` is a multi-stage build.

1.  **Builder Stage:** This stage uses the `gcc:latest` image to build the C++ application. It clones the Crow and spdlog repositories, installs the `hiredis`, `sqlite3`, and `libsodium` client libraries, and compiles the application.
2.  **Final Stage:** This stage uses a minimal `debian:stable-slim` image and copies the compiled application and its runtime dependencies from the builder stage. This results in a smaller final image.

The `docker-compose.yml` file defines two services:
*   `cpp-api`: The C++ application.
*   `redis`: The Redis server.
