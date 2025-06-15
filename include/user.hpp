#pragma once

#include <string>
// using namespace std;
class User {
public:
    User(std::string u, std::string p, std::string r);
    std::string username;
    std::string password;
    std::string role;
};

