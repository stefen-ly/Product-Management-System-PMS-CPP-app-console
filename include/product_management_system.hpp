#pragma once

#include <vector>
#include <iostream>
#include "user.hpp"
#include "product.hpp"
#include "category.hpp"
#include "order.hpp"
#include "restock_log.hpp"


class ProductManagementSystem {
private:
    std::vector<User> users;
    std::vector<Product> products;
    std::vector<Category> categories;
    std::vector<Order> orders;
    std::vector<RestockLog> restockLogs;
    std::string getCategoryName(const std::string& categoryUuid) const;
    User* currentUser;
    
    static int counter;
    
    // File constants
    const std::string USER_FILE = "users.csv";
    const std::string PRODUCT_FILE = "products.csv";
    const std::string CATEGORY_FILE = "categories.csv";
    const std::string ORDER_FILE = "orders.csv";
    const std::string RESTOCK_LOG_FILE = "restock_log.csv";
    
    // Data loading methods
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void loadOrders();
    void loadRestockLogs();
    
    // Data saving methods
    void saveUsers();
    void saveProducts();
    void saveCategories();
    void saveOrders();
    void saveRestockLogs();
    
    // ID generation methods
    std::string generateOrderId() const;
    std::string generateCategoryUuid();
    std::string generateProductId();
    
    // Utility methods
    bool isExpired(const Product& product);
    void exportDataToCsv();
    
    // Menu methods
    void adminDashboard();
    void staffDashboard();
    
    // Management methods
    void manageUsers();
    void manageCategories();
    void manageProducts();
    void restockProduct();
    void staffmanagProducts();
    
    // Product operations
    void addNewProduct();
    void viewAllProducts(int pageSize = 10);
    void searchProductMenu();
    void updateProduct();
    void deleteProduct();
    void viewLowStockProducts();
    void sortProductsMenu();
    void viewExpiredProducts();
    void viewHighStockProducts();
    
    // Order operations
    void placeOrder();
    void viewStaffOrders();
    void viewOrdersPlaced();
    
    // Report operations
    void generateReport();
    void exportProductReport();

    // User management
    void addUser();
    void updateUser();
    void deleteUser();
    void viewAllUsers();

    // Category management
    void addCategory();
    void updateCategory();
    void deleteCategory();
    void viewAllCategories();

    // Export methods
    void exportToCsv();
    void exportToTxt();

    // View methods
    void productByCategory();
    void viewProduct();

    // utility
    void presskeyEnter();

    // Order operation
    void addtoCart(Order& order);
    void viewCarts(Order& order);
    void updateCart(Order& order);
    void confirmCart(Order& order);

    void displayOrderDetails(Order& order);

public:
    ProductManagementSystem();
    void mainMenu();
    bool login(const std::string& role);

};