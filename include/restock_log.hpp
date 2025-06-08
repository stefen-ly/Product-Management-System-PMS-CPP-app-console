#pragma once

#include <string>
// using namespace std;

class RestockLog {
public:
    std::string productId;
    std::string productName;
    int quantityRestocked;
    std::string restockDate;
    
    RestockLog(std::string id, std::string name, int qty, std::string date);
};

