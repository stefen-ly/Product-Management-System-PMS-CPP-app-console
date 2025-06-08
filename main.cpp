#include <iostream>
#include "user.hpp"
#include "product.hpp"
#include "order.hpp"
#include "restock_log.hpp"
#include "product_management_system.hpp"
#include "category.hpp"
#include "utils.hpp"

using namespace std;

int main() {
    ProductManagementSystem pms;
    pms.mainMenu();
    return 0;
}
