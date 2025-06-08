#pragma once

#include <string>
// using namespace std;
class User {
public:
    std::string username;
    std::string password;
    std::string role; 
    
    User(std::string u, std::string p, std::string r);
};

