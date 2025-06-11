#pragma once
#include <string>
#include <vector>
#include <utility>
#include "product.hpp"
#include <ctime>

class Order {
public:
    std::string orderId;
    std::string customerName;
    std::string dateTime;
    std::vector<std::pair<Product, int>> products;
    double total;
    
    Order(std::string id, std::string c);

};

