// src/main.cpp

#include "Crow/include/crow.h" // Includes the core Crow framework

int main()
{
    // Create a Crow application instance
    crow::SimpleApp app;

    // Define a route for the root URL "/"
    // This route handles GET requests.
    CROW_ROUTE(app, "/")([](){
        // Create a JSON object for the response
        crow::json::wvalue response;
        response["status"] = "success";
        response["message"] = "Hello, World from C++ API!";
        return response;
    });

    // Set the port for the server to listen on.
    // We'll use 18080 inside the container.
    app.port(18080)
       .multithreaded() // Run the server on multiple threads for better performance
       .loglevel(crow::LogLevel::Warning)
       .run();          // Start the server

    return 0;
}