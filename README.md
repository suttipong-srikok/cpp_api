# C++ REST API with Crow

This is a simple C++ REST API built using the [Crow](https://github.com/CrowCpp/Crow) micro-framework. The application is containerized using Docker and provides full CRUD (Create, Read, Update, Delete) functionality for managing users.

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

    This command will build the Docker image and start the container. The API will be available at `http://localhost:18080`.

## API Endpoints

### Users

*   **`POST /users`**: Create a new user.

    **Request Body:**

    ```json
    {
        "name": "John Doe",
        "email": "john.doe@example.com"
    }
    ```

    **Example using `curl`:**

    ```bash
    curl -X POST -H "Content-Type: application/json" -d '{"name":"John Doe","email":"john.doe@example.com"}' http://localhost:18080/users
    ```

*   **`GET /users`**: Get a list of all users.

    **Example using `curl`:**

    ```bash
    curl http://localhost:18080/users
    ```

*   **`GET /users/<id>`**: Get a specific user by their ID.

    **Example using `curl`:**

    ```bash
    curl http://localhost:18080/users/1
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
    curl -X PUT -H "Content-Type: application/json" -d '{"name":"Jane Doe","email":"jane.doe@example.com"}' http://localhost:18080/users/1
    ```

*   **`DELETE /users/<id>`**: Delete a user.

    **Example using `curl`:**

    ```bash
    curl -X DELETE http://localhost:18080/users/1
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

1.  **Builder Stage:** This stage uses the `gcc:latest` image to build the C++ application. It clones the Crow repository and compiles the application.
2.  **Final Stage:** This stage uses a minimal `debian:stable-slim` image and copies the compiled application from the builder stage. This results in a smaller final image.

## Changelog

### `refactor: Use User struct for data model`

Refactored the application to use a `User` struct instead of `crow::json::wvalue` to store data. This improves type safety and code clarity. Added a helper function to convert the `User` struct to JSON for API responses.

### `feat: Initial commit of C++ REST API with Crow`

This commit introduces a basic C++ REST API using the Crow micro-framework. The application is containerized with Docker and includes the necessary configuration for building and running.

Key features:
- A simple "Hello, World" endpoint at `/`.
- A multi-stage Dockerfile for an optimized build.
- A `docker-compose.yml` for easy startup.
- A `.gitignore` file for C++ and Docker projects.
- A `README.md` with setup and usage instructions.
