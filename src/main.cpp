// src/main.cpp

#include "Crow/include/crow.h"
#include <vector>
#include <mutex>
#include <string>

// Define a struct to hold our user data
struct User {
    int id;
    std::string name;
    std::string email;
};

// Helper function to convert a User struct to a crow::json::wvalue
crow::json::wvalue user_to_json(const User& user) {
    crow::json::wvalue json_user;
    json_user["id"] = user.id;
    json_user["name"] = user.name;
    json_user["email"] = user.email;
    return json_user;
}

// A simple in-memory "database" for our users.
std::vector<User> users;
// A mutex to protect access to the shared 'users' vector from multiple threads.
std::mutex mtx;
// A counter to generate unique user IDs.
int next_user_id = 1;

int main()
{
    crow::SimpleApp app;

    // --- CREATE: Add a new user (POST /users) ---
    CROW_ROUTE(app, "/users").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("name") || !body.has("email")) {
            return crow::response(400, "{\"message\": \"Invalid JSON or missing fields\"}");
        }

        User new_user;
        new_user.name = body["name"].s();
        new_user.email = body["email"].s();

        std::lock_guard<std::mutex> guard(mtx);
        new_user.id = next_user_id++;
        users.push_back(new_user);

        return crow::response(201, user_to_json(new_user));
    });

    // --- READ: Get all users (GET /users) ---
    CROW_ROUTE(app, "/users").methods("GET"_method)
    ([](){
        std::lock_guard<std::mutex> guard(mtx);
        std::vector<crow::json::wvalue> json_users;
        for (const auto& user : users) {
            json_users.push_back(user_to_json(user));
        }
        return crow::response(200, crow::json::wvalue(json_users));
    });

    // --- READ: Get a specific user by ID (GET /users/<id>) ---
    CROW_ROUTE(app, "/users/<int>").methods("GET"_method)
    ([](int user_id){
        std::lock_guard<std::mutex> guard(mtx);
        for (const auto& user : users) {
            if (user.id == user_id) {
                return crow::response(200, user_to_json(user));
            }
        }
        return crow::response(404, "{\"message\": \"User not found\"}");
    });

    // --- UPDATE: Modify a user's data (PUT /users/<id>) ---
    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)
    ([](const crow::request& req, int user_id){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "{\"message\": \"Invalid JSON\"}");
        }

        std::lock_guard<std::mutex> guard(mtx);
        for (auto& user : users) {
            if (user.id == user_id) {
                if (body.has("name")) user.name = body["name"].s();
                if (body.has("email")) user.email = body["email"].s();
                return crow::response(200, user_to_json(user));
            }
        }
        return crow::response(404, "{\"message\": \"User not found\"}");
    });

    // --- DELETE: Remove a user by ID (DELETE /users/<id>) ---
    CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)
    ([](int user_id){
        std::lock_guard<std::mutex> guard(mtx);
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->id == user_id) {
                users.erase(it);
                return crow::response(200, "{\"message\": \"User deleted\"}");
            }
        }
        return crow::response(404, "{\"message\": \"User not found\"}");
    });

    // Configure and run the server
    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}