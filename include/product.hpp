#pragma once

#include <string>
// using namespace std;    
class Product {
public:
    std::string id;
    std::string name;
    std::string categoryUuid;
    double basePrice;
    double sellPrice;
    double costPrice;
    int quantity;
    std::string expirationDate;
    std::string creationDate;
    double totalPrice;
    std::string status;
    std::string size;
    
    Product(std::string id, std::string n, std::string c, double bp, double cp,
            int q, std::string e, std::string cd, double tp, std::string s, std::string sz);
};
