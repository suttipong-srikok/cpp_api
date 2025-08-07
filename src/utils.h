#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "crow.h"

// Define a struct to hold our user data
struct User {
    int id;
    std::string name;
    std::string email;
    std::string password;
};

// Helper function to convert a User struct to a crow::json::wvalue
crow::json::wvalue user_to_json(const User& user) {
    crow::json::wvalue json_user;
    json_user["id"] = user.id;
    json_user["name"] = user.name;
    json_user["email"] = user.email;
    return json_user;
}

#endif // UTILS_H