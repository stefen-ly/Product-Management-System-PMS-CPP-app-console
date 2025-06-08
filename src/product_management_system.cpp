#include "product_management_system.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
#include <vector>
#include <conio.h>   
#include <windows.h> 
#include <ctime>
#include <unordered_map>

using namespace std;
using namespace tabulate;

int ProductManagementSystem::counter = 0;

ProductManagementSystem::ProductManagementSystem() : currentUser(nullptr) {
    loadUsers();
    loadProducts();
    loadCategories();
    loadOrders();
    loadRestockLogs();
    
    if (users.empty()) {
        users.emplace_back("admin", "admin123", "Admin");
        saveUsers();
    }
}

string getHiddenInput(const string& prompt) {
    string input;
    char ch;

    cout << prompt;
    while ((ch = _getch()) != '\r') { 
        if (ch == '\b') { 
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b"; 
            }
        } else {
            input += ch;
            cout << '*';
        }
    }
    cout << endl;
    return input;
}

void ProductManagementSystem::mainMenu() {
    int selected = 0;
    const int options = 3;
    string menu[] = {
        "1️⃣  Admin Login",
        "2️⃣  Staff Login",
        "🅾️  Exit"
    };

    while (true) {
        system("cls"); 
        cout << "\n========== PRODUCT MANAGEMENT SYSTEM ==========\n";

        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }

        cout << "==============================================\n";
        cout << "↕️   Use arrow keys and press Enter to select.";

        int ch = _getch();
        if (ch == 224) { 
            switch (_getch()) {
                case 72: selected = (selected - 1 + options) % options; break; 
                case 80: selected = (selected + 1) % options; break;           
            }
        } else if (ch == 13) { 
            if (selected == 0) { 
                if (login("Admin") && currentUser->role == "Admin") {
                    adminDashboard();
                }
            } else if (selected == 1) {
                if (login("Staff") && currentUser->role == "Staff") {
                    staffDashboard();
                }
            } else if (selected == 2) { 
                break;
            }
        }
    }
}

bool ProductManagementSystem::login(const string& role) {
    string username, password;
    int attempts = 3;

    while (attempts > 0) {
        cout << "\n=========== LOGIN ===========\n";
        username = getHiddenInput("Enter Username: ");
        password = getHiddenInput("Enter Password: ");
        cout << "=============================\n";

        for (auto& user : users) {
            if (user.username == username && user.password == password && user.role == role) {
                currentUser = &user;
                cout << "Login successful!\n";
                return true;
            }
        }

        attempts--;
        cout << "Invalid credentials! " << attempts << " attempts remaining.\n";
    }

    cout << "Max login attempts reached!\n";
    return false;
}

void ProductManagementSystem::adminDashboard() {
    int selected = 0;
    const int options = 7;
    string menu[] = {
        "1️⃣   Manage Users",
        "2️⃣   Manage Categories",
        "3️⃣   Manage Products",
        "4️⃣   Generate Report",
        "5️⃣   View Staff Orders",
        "6️⃣   Restock Products",
        "🅾️   Logout"
    };

    while (true) {
        system("cls"); 
        cout << "\n============= ADMIN DASHBOARD =============\n";
        
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }

        cout << "===========================================\n";
        cout << "↕️   Use arrow to navigate.";

        int ch = _getch();
        if (ch == 224) {
            switch (_getch()) {
                case 72: selected = (selected - 1 + options) % options; break; 
                case 80: selected = (selected + 1) % options; break;           
            }
        } else if (ch == 13) { 
            switch (selected) {
                case 0: manageUsers(); break;
                case 1: manageCategories(); break;
                case 2: manageProducts(); break;
                case 3: generateReport(); break;
                case 4: viewStaffOrders();
                        presskeyEnter();
                    break;
                case 5: restockProduct(); break;
                case 6:
                    currentUser = nullptr;
                    return;
            }
        }
    }
}

void ProductManagementSystem::staffmanagProducts(){
    int selected = 0;
    const int options = 7;
    string menu[] = {
        "1️⃣   Add New Product",
        "2️⃣   View All Products",
        "3️⃣   Search Product",
        "4️⃣   Update Product",
        "5️⃣   View Low Stock Products",
        "6️⃣   Sort Products",
        "🅾️   Back to Staff Dashboard"
    };

    while (true) {
        system("cls");
        cout << "\n========== PRODUCT MANAGEMENT ==========\n";
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }
        cout << "========================================\n";
        cout << "↕️   Use arrow to navigate and Enter to select.";

        int ch = _getch();
        if (ch == 224) {
            switch (_getch()) {
                case 72: selected = (selected - 1 + options + 1) % (options + 1); break;
                case 80: selected = (selected + 1) % (options + 1); break;
            }
        } else if (ch == 13) {
            switch (selected) {
                case 0: addNewProduct(); break;
                case 1: viewProduct(); break;
                case 2: searchProductMenu(); break;
                case 3: updateProduct(); 
                        presskeyEnter();
                break;
                case 4: viewLowStockProducts(); 
                        presskeyEnter();
                break;
                case 5: sortProductsMenu(); break;
                case 6: return;
            }
        }
    }
}

void ProductManagementSystem::staffDashboard() {
    int selected = 0;
    const int options = 4;
    string menu[] = {
        "1️⃣    Product Management",
        "2️⃣    Place an Order for the Customer",
        "3️⃣    View Orders Placed",
        "🅾️    Logout"
    };

    while (true) {
        system("cls");
        cout << "\n============= STAFF DASHBOARD =============\n";
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }
        cout << "===========================================\n";
        cout << "↕️   Use arrow to navigate and Enter to select.";

        int ch = _getch();
        if (ch == 224) {
            switch (_getch()) {
                case 72: selected = (selected - 1 + options) % options; break;
                case 80: selected = (selected + 1) % options; break;
            }
        } else if (ch == 13) {
            switch (selected) {
                case 0:
                    staffmanagProducts();
                    break;
                case 1:
                    placeOrder();
                    break;
                case 2:
                    viewStaffOrders();
                    presskeyEnter();
                    break;
                case 3:
                    currentUser = nullptr;
                    return;
            }
        }
    }
}

void ProductManagementSystem::loadUsers() {
    ifstream file(USER_FILE);
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); 
    
    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        if (fields.size() >= 3) {
            users.emplace_back(fields[0], fields[1], fields[2]);
        }
    }
    file.close();
}

void ProductManagementSystem::loadProducts() {
    ifstream file(PRODUCT_FILE);
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); 
    
    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        try {
            if (fields.size() >= 11) { 
                double basePrice = stod(fields[3]);
                double sellPrice = stod(fields[4]);
                int quantity = stoi(fields[5]);
                double totalPrice = stod(fields[8]);
                string status = fields[9];
                if (status != "Available" && status != "false") {
                    status = (quantity > 0 ? "Available" : "false");
                }
                products.emplace_back(fields[0], fields[1], fields[2], basePrice, sellPrice, 
                                    quantity, fields[6], fields[7], totalPrice, status, fields[10]);
            } else if (fields.size() >= 10) { 
                double basePrice = stod(fields[3]);
                double sellPrice = stod(fields[4]);
                int quantity = stoi(fields[5]);
                double totalPrice = stod(fields[8]);
                string status = fields[9];
                if (status != "Available" && status != "false") {
                    status = (quantity > 0 ? "Available" : "false");
                }
                products.emplace_back(fields[0], fields[1], fields[2], basePrice, sellPrice, 
                                    quantity, fields[6], fields[7], totalPrice, status, "");
                cout << "Warning: Converted product entry for ID " << fields[0] << " to include size (set to empty)\n";
            } else if (fields.size() >= 7) { 
                double basePrice = stod(fields[3]);
                double sellPrice = stod(fields[4]);
                int quantity = stoi(fields[5]);
                double totalPrice = sellPrice * quantity;
                string status = quantity > 0 ? "Available" : "";
                string creationDate = "2025-01-01";
                products.emplace_back(fields[0], fields[1], fields[2], basePrice, sellPrice, 
                                    quantity, fields[6], creationDate, totalPrice, status, "");
                cout << "Warning: Converted product entry for ID " << fields[0] << " to include creationDate, totalPrice, status, size\n";
            } else if (fields.size() >= 6) {
                double price = stod(fields[3]);
                int quantity = stoi(fields[4]);
                double basePrice = price * 0.8;
                double sellPrice = price;
                double totalPrice = sellPrice * quantity;
                string status = quantity > 0 ? "Available" : "";
                string creationDate = "2025-01-01";
                products.emplace_back(fields[0], fields[1], fields[2], basePrice, sellPrice, 
                                    quantity, fields[5], creationDate, totalPrice, status, "");
                cout << "Warning: Converted older product entry for ID " << fields[0] << "\n";
            } else {
                cout << "Warning: Skipping malformed product line: " << line << endl;
            }
        } catch (...) {
            cout << "Warning: Skipping malformed product line: " << line << endl;
        }
    }
    file.close();
}

void ProductManagementSystem::loadCategories() {
    ifstream file(CATEGORY_FILE);
    static int maxCounter = 0;
    
    if (!file.is_open()) {
        maxCounter = 0;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        if (fields.size() >= 2) {
            categories.emplace_back(fields[0], fields[1]);
            if (fields[0].substr(0, 4) == "CAT-") {
                try {
                    int num = stoi(fields[0].substr(4));
                    if (num > maxCounter) {
                        maxCounter = num;
                    }
                } catch (...) {
                    cout << "Warning: Invalid category format: " << fields[0] << endl;
                }
            }
        }
    }
    file.close();
    counter = maxCounter;
}

void ProductManagementSystem::loadOrders() {
    ifstream file(ORDER_FILE);
    if (!file.is_open()) return;
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        if (fields.size() >= 4) {
            try {
                Order order(fields[0], fields[1]);
                order.total = stod(fields[2]);
                string productsStr = fields[3];
                stringstream ss(productsStr);
                string productEntry;
                
                while (getline(ss, productEntry, ';')) {
                    if (productEntry.empty()) continue;
                    size_t pos = productEntry.find(':');
                    if (pos == string::npos) continue;
                    
                    string productId = productEntry.substr(0, pos);
                    int qty = stoi(productEntry.substr(pos + 1));
                    bool found = false;
                    
                    for (const auto& p : products) {
                        if (p.id == productId) {
                            order.products.emplace_back(p, qty);
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        cout << "Warning: Product ID " << productId << " not found in order " << order.orderId << endl;
                    }
                }
                orders.push_back(order);
            } catch (...) {
                cout << "Warning: Skipping malformed order line: " << line << endl;
            }
        }
    }
    file.close();
}

void ProductManagementSystem::loadRestockLogs() {
    ifstream file(RESTOCK_LOG_FILE);
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); 
    
    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        if (fields.size() >= 4) {
            try {
                int quantityRestocked = stoi(fields[2]);
                restockLogs.emplace_back(fields[0], fields[1], quantityRestocked, fields[3]);
            } catch (...) {
                cout << "Warning: Skipping malformed restock log line: " << line << endl;
            }
        }
    }
    file.close();
}

void ProductManagementSystem::saveUsers() {
    ofstream file(USER_FILE);
    file << "Username,Password,Role\n";
    
    for (const auto& user : users) {
        string escapedUsername = user.username;
        string escapedPassword = user.password;
        string escapedRole = user.role;
        
        replace(escapedUsername.begin(), escapedUsername.end(), ',', ';');
        replace(escapedPassword.begin(), escapedPassword.end(), ',', ';');
        replace(escapedRole.begin(), escapedRole.end(), ',', ';');
        
        file << "\"" << escapedUsername << "\",\"" << escapedPassword << "\",\"" << escapedRole << "\"\n";
    }
    file.close();
}

void ProductManagementSystem::saveProducts() {
    ofstream file(PRODUCT_FILE);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << PRODUCT_FILE << " for writing!\n";
        return;
    }
    
    file << "ID,Name,CategoryUUID,BasePrice,SellPrice,Quantity,Expiration,CreationDate,TotalPrice,Status,Size\n";
    
    for (const auto& product : products) {
        string escapedName = product.name;
        string escapedCategoryUuid = product.categoryUuid;
        string escapedExpiration = product.expirationDate;
        string escapedCreationDate = product.creationDate;
        string escapedSize = product.size;
        
        replace(escapedName.begin(), escapedName.end(), ',', ';');
        replace(escapedCategoryUuid.begin(), escapedCategoryUuid.end(), ',', ';');
        replace(escapedExpiration.begin(), escapedExpiration.end(), ',', ';');
        replace(escapedCreationDate.begin(), escapedCreationDate.end(), ',', ';');
        replace(escapedSize.begin(), escapedSize.end(), ',', ';');
        
        file << "\"" << product.id << "\",\"" << escapedName << "\",\"" << escapedCategoryUuid << "\","
             << fixed << setprecision(2) << product.basePrice << "," << product.sellPrice << ","
             << product.quantity << ",\"" << escapedExpiration << "\",\"" << escapedCreationDate << "\","
             << product.totalPrice << ",\"" << product.status << "\",\"" << escapedSize << "\"\n";
    }
    file.close();
}

void ProductManagementSystem::saveCategories() {
    ofstream file(CATEGORY_FILE);
    file << "UUID,Name\n";
    
    for (const auto& category : categories) {
        string escapedName = category.name;
        replace(escapedName.begin(), escapedName.end(), ',', ';');
        file << "\"" << category.uuid << "\",\"" << escapedName << "\"\n";
    }
    file.close();
}

void ProductManagementSystem::saveOrders() {
    ofstream file(ORDER_FILE);
    file << "OrderID,CustomerName,Products,Total,Date\n";
    
    for (const auto& order : orders) {
        string escapedCustomerName = order.customerName;
        replace(escapedCustomerName.begin(), escapedCustomerName.end(), ',', ';');
        
        string productsStr;
        for (size_t i = 0; i < order.products.size(); ++i) {
            productsStr += order.products[i].first.name + ":" + to_string(order.products[i].second);
            if (i < order.products.size() - 1) productsStr += ";";
        }

        file << "\"" << order.orderId << "\","
             << "\"" << escapedCustomerName << "\","
             << "\"" << productsStr << "\","
             << fixed << setprecision(2) << order.total << ","
             << "\"" << order.dateTime << "\"\n";
    }
    
    file.close();
}

void ProductManagementSystem::saveRestockLogs() {
    ofstream file(RESTOCK_LOG_FILE);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << RESTOCK_LOG_FILE << " for writing!\n";
        return;
    }
    
    file << "ProductID,ProductName,QuantityRestocked,RestockDate\n";
    
    for (const auto& log : restockLogs) {
        string escapedProductName = log.productName;
        replace(escapedProductName.begin(), escapedProductName.end(), ',', ';');
        
        file << "\"" << log.productId << "\",\"" << escapedProductName << "\","
             << log.quantityRestocked << ",\"" << log.restockDate << "\"\n";
    }
    file.close();
}

string ProductManagementSystem::generateOrderId() const {
    return "ORD-" + to_string(orders.size() + 1);
}

string ProductManagementSystem::generateCategoryUuid() {
    return "CAT-" + to_string(++counter);
}

string ProductManagementSystem::generateProductId() {
    int counter = products.size() + 1;
    string id;
    bool unique;
    
    do {
        unique = true;
        stringstream ss;
        ss << "P" << setw(3) << setfill('0') << counter++;
        id = ss.str();
        
        for (const auto& product : products) {
            if (product.id == id) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    
    return id;
}

// bool ProductManagementSystem::isExpired(const Product& product) {
//     std::tm exp_tm = {};
//     std::istringstream ss(product.expirationDate);
//     ss >> std::get_time(&exp_tm, "%Y-%m-%d");
//     if (ss.fail()) {
//         return false;
//     }
//     std::time_t exp_time = std::mktime(&exp_tm);

//     std::time_t now_time = std::time(nullptr);
//     std::tm* now_tm = std::localtime(&now_time);
//     now_tm->tm_hour = 0;
//     now_tm->tm_min = 0;
//     now_tm->tm_sec = 0;
//     std::time_t today_time = std::mktime(now_tm);

//     return exp_time < today_time;
// }

bool ProductManagementSystem::isExpired(const Product& product) {
    std::tm expTime = {};
    std::istringstream ss(product.expirationDate);
    ss >> std::get_time(&expTime, "%d-%m-%Y");
    if (ss.fail()) return false;

    std::time_t exp = std::mktime(&expTime);

    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    now_tm->tm_hour = 0;
    now_tm->tm_min = 0;
    now_tm->tm_sec = 0;
    std::time_t today = std::mktime(now_tm);

    return exp < today;
}

void ProductManagementSystem::addUser(){
    string username, password, role;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter role (Admin/Staff): ";
    getline(cin, role);
                
    if (role != "Admin" && role != "Staff") {
        cout << "Invalid role!\n";
        return;
    }
                
    users.emplace_back(username, password, role);
    saveUsers();
    cout << "User added successfully!\n";
}
void ProductManagementSystem::updateUser() {
    if (users.empty()) {
        cout << "No users available to update!\n";
        return;
    }
    string username, password, role;
    cout << "Enter username to update: ";
    getline(cin, username);
                
    for (auto& user : users) {
        if (user.username == username) {
            cout << "Enter new password (or press Enter to keep current): ";
            getline(cin, password);
            if (!password.empty()) user.password = password;
                        
            cout << "Enter new role (Admin/Staff, or press Enter to keep current): ";
            getline(cin, role);
            if (!role.empty() && (role == "Admin" || role == "Staff")) {
                user.role = role;
            }
                        
            saveUsers();
            cout << "User updated successfully!\n";
            return;
        }
    }
    cout << "User not found!\n";
}
void ProductManagementSystem::deleteUser() {
    if (users.empty()) {
        cout << "No users available to delete!\n";
        return;
    }
    string username;
    cout << "Enter username to delete: ";
    getline(cin, username);
                
    auto it = remove_if(users.begin(), users.end(),
        [&username](const User& u) { return u.username == username; });
                
    if (it != users.end()) {
        users.erase(it, users.end());
        saveUsers();
        cout << "User deleted successfully!\n";
    } else {
        cout << "User not found!\n";
    }
}
void ProductManagementSystem::viewAllUsers() {
    if (users.empty()) {
        cout << "No users found!\n";
        return;
    }

    cout << "👨‍💼   User List... " <<endl;
    Table table;
    table.add_row({"No", "Username", "Passwords", "Role"});

    int count = 1;
    for (const auto& user : users) {
        table.add_row({
            to_string(count),
            user.username,
            user.password,
            user.role
        });
        count++;
    }

    table[0].format().font_style({FontStyle::bold});
    cout << table << endl;
}
void ProductManagementSystem::manageUsers() {
    const int MENU_SIZE = 5;
    string options[MENU_SIZE] = {
        "1️⃣    Add New User",
        "2️⃣    View All Users",
        "3️⃣    Update User Role or Password",
        "4️⃣    Delete User",
        "🅾️    Back to Admin Dashboard"
    };

    int selected = 0;
    while (true) {
        system("cls"); 
        cout << "\n=========== USER MANAGEMENT ===========\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "=======================================\n";
        cout << "↕️   Use arrows to navigate, ENTER to select.";

        int ch = _getch();
        if (ch == 224) { 
            ch = _getch(); 
            switch (ch) {
                case 72:
                    selected = (selected - 1 + MENU_SIZE) % MENU_SIZE;
                    break;
                case 80: 
                    selected = (selected + 1) % MENU_SIZE;
                    break;
            }
        } else if (ch == 13) { 
            system("cls");
            switch (selected) {
                case 0: addUser(); break;
                case 1: viewAllUsers(); 
                        presskeyEnter();
                break;
                case 2: updateUser(); break;
                case 3: deleteUser(); break;
                case 4: return; 
            }
        }
    }
}

void ProductManagementSystem::addCategory() {
    string name;
    cout << "Enter category name: ";
    getline(cin, name);
    
    string uuid = generateCategoryUuid();
    categories.emplace_back(uuid, name);
    saveCategories();
    cout << "Category added successfully! UUID: " << uuid << endl;
}

void ProductManagementSystem::updateCategory() {
    if (categories.empty()) {
        cout << "No categories available to update!\n";
        return;
    }
    
    string uuid, newName;
    cout << "Enter category UUID to update: ";
    getline(cin, uuid);
    
    for (auto& category : categories) {
        if (category.uuid == uuid) {
            cout << "Enter new name: ";
            getline(cin, newName);
            category.name = newName;
            saveCategories();
            cout << "Category updated successfully!\n";
            return;
        }
    }
    
    cout << "Category not found!\n";
}

void ProductManagementSystem::viewAllCategories() {
    if (categories.empty()) {
        cout << "‼️No categories found!\n";
        return;
    }

    cout << "📋 Category Lists..." <<endl;
    Table table;
    table.add_row({"No", "UUID", "Name"});

    int count = 1;
    for (const auto& category : categories) {
        table.add_row({
            to_string(count),
            category.uuid,
            category.name
        });
        count++;
    }

    table[0].format().font_style({FontStyle::bold});
    cout << table << endl;
}

void ProductManagementSystem::deleteCategory() {
    if (categories.empty()) {
        cout << "‼️No categories available to delete!\n";
        return;
    }
    
    string uuid;
    cout << "Enter category UUID to delete: ";
    getline(cin, uuid);
    
    auto it = remove_if(categories.begin(), categories.end(),
        [&uuid](const Category& c) { return c.uuid == uuid; });
    
    if (it != categories.end()) {
        categories.erase(it, categories.end());
        saveCategories();
        cout << "Category deleted successfully!\n";
    } else {
        cout << "Category not found!\n";
    }
}

void ProductManagementSystem::manageCategories() {
    const int MENU_SIZE = 5;
    string options[MENU_SIZE] = {
        "1️⃣    Add New Category",
        "2️⃣    View All Categories",
        "3️⃣    Update Category",
        "4️⃣    Delete Category",
        "🅾️    Back to Admin Dashboard"
    };

    int selected = 0;
    while (true) {
        system("cls");
        cout << "\n=========== CATEGORY MANAGEMENT ===========\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "===========================================\n";
        cout << "↕️  Use arrows to navigate, ENTER to select.\n";

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72)       selected = (selected - 1 + MENU_SIZE) % MENU_SIZE; 
            else if (ch == 80)  selected = (selected + 1) % MENU_SIZE;            
        } else if (ch == 13) {
            system("cls");
            switch (selected) {
                case 0: addCategory(); break;
                case 1: viewAllCategories();
                        presskeyEnter();
                 break;
                case 2: updateCategory(); break;
                case 3: deleteCategory(); break;
                case 4: return; 
            }
        }
    }
}

void ProductManagementSystem::manageProducts() {
    int selected = 0;
    const int optionCount = 9;

    string menu[] = {
        "1️⃣  Add New Product",
        "2️⃣  View All Products",
        "3️⃣  Search Product",
        "4️⃣  Update Product",
        "5️⃣  Delete Product",
        "6️⃣  View Low Stock Products",
        "7️⃣  Sort Products",
        "8️⃣  View Expired Products",
        "🅾️  Back to Dashboard"
    };

    while (true) {
        system("cls");
        cout << "\n=========== PRODUCT MANAGEMENT ===========\n";
        for (int i = 0; i < optionCount; ++i) {
            if (i == selected){
                cout << "👉 " << menu[i] << "\n";
            } else {
                cout << "   " << menu[i] << "\n";
            } 
        }
        cout << "==========================================\n";
        cout << "↕️   Use arrow to navigate";

        int ch = _getch();
        if (ch == 224) {
            switch (_getch()) {
                case 72: selected = (selected - 1 + optionCount) % optionCount; break; 
                case 80: selected = (selected + 1) % optionCount; break;         
            }
        } else if (ch == 13) {
            switch (selected) {
                case 0: addNewProduct(); break;
                case 1: viewProduct(); 
                break;
                case 2: searchProductMenu(); break;
                case 3: updateProduct(); break;
                case 4: deleteProduct(); 
                presskeyEnter();
                break;
                case 5: viewLowStockProducts(); 
                    presskeyEnter();
                break;
                case 6: sortProductsMenu(); break;
                case 7: viewExpiredProducts(); break;
                case 8: return;
            }
        }
    }
}

// void ProductManagementSystem::addNewProduct() {
//     while (true) {
//         if (categories.empty()) {
//             cout << "‼️No categories available! Please add a category first.\n";
//             return;
//         }

//         viewAllCategories();

//         string categoryUuid;
//         cout << "Enter category UUID: ";
//         getline(cin, categoryUuid);

//         bool validCategory = false;
//         for (const auto& category : categories) {
//             if (category.uuid == categoryUuid) {
//                 validCategory = true;
//                 break;
//             }
//         }
        
//         if (!validCategory) {
//             cout << "‼️Invalid category UUID!\n";
//             return;
//         }

//         string id = generateProductId();
//         string name, expiration, size;
//         double basePrice, sellPrice;
//         int quantity;
        
//         cout << "Generated Product ID: " << id << endl;
//         cout << "Enter product name: ";
//         getline(cin, name);
//         cout << "Enter base price (cost): ";
//         cin >> basePrice;
//         cout << "Enter sell price (retail): ";
//         cin >> sellPrice;
        
//         if (basePrice > sellPrice) {
//             cout << "‼️Error: Base price cannot exceed sell price!\n";
//             return;
//         }
        
//         cout << "Enter quantity: ";
//         cin >> quantity;
//         cin.ignore();
//         cout << "Enter expiration date (YYYY-MM-DD): ";
//         getline(cin, expiration);
//         cout << "Enter size (press Enter to skip): ";
//         getline(cin, size);

//         string creationDate = getCurrentDate();
//         double totalPrice = sellPrice * quantity;
//         string status = quantity > 0 ? "Available" : "false";

//         products.emplace_back(id, name, categoryUuid, basePrice, sellPrice, quantity, 
//                              expiration, creationDate, totalPrice, status, size);
//         saveProducts();
//         cout << "Product added successfully with ID: " << id << "!\n";

//         char addMore;
//         cout << "Add another product? (Y/N): ";
//         cin >> addMore;
//         cin.ignore();
        
//         if (addMore != 'Y' && addMore != 'y') {
//             break;
//         }
//     }
// }

void ProductManagementSystem::addNewProduct() {
    while (true) {
        if (categories.empty()) {
            cout << "‼️No categories available! Please add a category first.\n";
            return;
        }

        viewAllCategories();

        string categoryUuid;
        bool validCategory = false;
        do {
            cout << "Enter category UUID (or 'q' to cancel): ";
            getline(cin, categoryUuid);
            if (categoryUuid == "q" || categoryUuid == "Q") {
                return; 
            }

            validCategory = false;
            for (const auto& category : categories) {
                if (category.uuid == categoryUuid) {
                    validCategory = true;
                    break;
                }
            }
            if (!validCategory) {
                cout << "‼️Invalid category UUID! Please try again.\n";
            }
        } while (!validCategory);

        string id = generateProductId();
        string name, expiration, size;
        double basePrice = 0.0, sellPrice = 0.0;
        int quantity = 0;

        cout << "Generated Product ID: " << id << endl;
        cout << "Enter product name: ";
        getline(cin, name);

        // Input and validate basePrice
        cout << "Enter base price (cost): ";
        while (!(cin >> basePrice) || basePrice < 0) {
            cout << "Invalid input. Enter a non-negative number for base price: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter sell price (retail): ";
        while (!(cin >> sellPrice) || sellPrice < 0) {
            cout << "Invalid input. Enter a non-negative number for sell price: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (basePrice > sellPrice) {
            cout << "‼️Error: Base price cannot exceed sell price!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter quantity: ";
        while (!(cin >> quantity) || quantity < 0) {
            cout << "Invalid input. Enter a non-negative integer for quantity: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(); 
        expiration = inputExpireDate();
        cout << "Enter size (press Enter to skip): ";
        getline(cin, size);

        string creationDate = getCurrentDate();
        double totalPrice = sellPrice * quantity;
        string status = quantity > 0 ? "Available" : "Unavailable";

        products.emplace_back(id, name, categoryUuid, basePrice, sellPrice, quantity,
                             expiration, creationDate, totalPrice, status, size);
        saveProducts();
        cout << "Product added successfully with ID: " << id << "!\n";

        char addMore;
        cout << "Add another product? (Y/N): ";
        cin >> addMore;
        cin.ignore();

        if (addMore != 'Y' && addMore != 'y') {
            break;
        }
    }
}

string ProductManagementSystem::getCategoryName(const string& categoryUuid) const {
    for (const auto& category : categories) {
        if (category.uuid == categoryUuid) {
            return category.name;
        }
    }
    return "Unknown";
}

void ProductManagementSystem::viewAllProducts(int pageSize) {
    if (products.empty()) {
        cout << "\n‼️ No products available!\n";
        return;
    }

    size_t totalPages = (products.size() + pageSize - 1) / pageSize; 
    size_t currentPage = 0;

    while (true) {
        system("cls"); 
        cout << "🛒 Product List (Page " << currentPage + 1 << " of " << totalPages << ") 🛒\n\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        size_t start = currentPage * pageSize;
        size_t end = min(start + pageSize, products.size());
        int count = start + 1; 

        for (size_t i = start; i < end; ++i) {
            const auto& product = products[i];
            string categoryName = getCategoryName(product.categoryUuid); 
            string displaySize = product.size.empty() ? "N/A" : product.size;

            ostringstream basePriceStream, sellPriceStream, totalPriceStream;
            basePriceStream << fixed << setprecision(2) << product.basePrice;
            sellPriceStream << fixed << setprecision(2) << product.sellPrice;
            totalPriceStream << fixed << setprecision(2) << product.totalPrice;

            table.add_row({
                to_string(count++),
                product.id,
                product.name,
                categoryName,
                displaySize,
                basePriceStream.str(),
                sellPriceStream.str(),
                to_string(product.quantity),
                product.creationDate,
                product.expirationDate,
                totalPriceStream.str(),
                product.status
            });
        }

        table[0].format().font_style({FontStyle::bold});
        cout << table << endl;
        cout << "Page " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "[⬅️  ] Prev Page [➡️  ] Next Page [ESC] Back";

        int key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 75) { 
                if (currentPage > 0) currentPage--;
                else {
                    cout << "\nAlready on the first page!\n";
                }
            } else if (key == 77) {
                if (currentPage < totalPages - 1) currentPage++;
                else {
                    cout << "\nAlready on the last page!\n";
                }
            }
        } else if (key == 27) {
            break;
        } else {
            cout << "\nInvalid key! Use ←, →, or ESC.\n";
        }
    }
}

void ProductManagementSystem::productByCategory() {
    if (categories.empty()) {
        cout << "‼️ No categories available!\n";
        return;
    }

    viewAllCategories();
    cout << "🆔 Enter category UUID: ";
    string categoryUuid;
    getline(cin, categoryUuid);

    bool validCategory = false;
    string selectedCategoryName = "Unknown";

    for (const auto& category : categories) {
        if (category.uuid == categoryUuid) {
            validCategory = true;
            selectedCategoryName = category.name;
            break;
        }
    }

    if (!validCategory) {
        cout << "‼️ Invalid category UUID!\n";
        return;
    }

    vector<Product> filteredProducts;
    for (const auto& product : products) {
        if (product.categoryUuid == categoryUuid) {
            filteredProducts.push_back(product);
        }
    }

    if (filteredProducts.empty()) {
        cout << "‼️ No products found in this category!\n";
        presskeyEnter();
        return;
    }

    const int pageSize = 5;
    size_t totalPages = (filteredProducts.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        system("cls");
        cout << "🛒 Products in Category: " << selectedCategoryName 
             << " (Page " << currentPage + 1 << " of " << totalPages << ")\n\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        size_t start = currentPage * pageSize;
        size_t end = min(start + pageSize, filteredProducts.size());
        int count = start + 1;

        for (size_t i = start; i < end; ++i) {
            const auto& product = filteredProducts[i];
            string displaySize = product.size.empty() ? "N/A" : product.size;

            ostringstream basePriceStream, sellPriceStream, totalPriceStream;
            basePriceStream << fixed << setprecision(2) << product.basePrice;
            sellPriceStream << fixed << setprecision(2) << product.sellPrice;
            totalPriceStream << fixed << setprecision(2) << product.totalPrice;

            table.add_row({
                to_string(count++),
                product.id,
                product.name,
                selectedCategoryName,
                displaySize,
                basePriceStream.str(),
                sellPriceStream.str(),
                to_string(product.quantity),
                product.creationDate,
                product.expirationDate,
                totalPriceStream.str(),
                product.status
            });
        }

        table[0].format().font_style({FontStyle::bold});
        cout << table << endl;

        cout << "Page " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "[⬅️  ] Prev Page [➡️  ] Next Page [ESC] Back";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 75) { 
                if (currentPage > 0) currentPage--;
                else {
                    cout << "\nAlready on the first page!\n";
                }
            } else if (key == 77) { 
                if (currentPage < totalPages - 1) currentPage++;
                else {
                    cout << "\nAlready on the last page!\n";
                }
            }
        } else if (key == 27) { 
            break;
        } else {
            cout << "\nInvalid key! Use ←, →, or ESC.\n";
        }
    }
}


void ProductManagementSystem::viewProduct() {
    const int MENU_SIZE = 3;
    string options[MENU_SIZE] = {
        "1️⃣   View All Products",
        "2️⃣   View by Category",
        "🅾️   Back to Dashboard"
    };

    int selected = 0;

    while (true) {
        system("cls");
        cout << "\n========== VIEW PRODUCTS ==========\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "=====================================\n";
        cout << "↕️   Use arrows to navigate";

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72)       selected = (selected - 1 + MENU_SIZE) % MENU_SIZE; 
            else if (ch == 80)  selected = (selected + 1) % MENU_SIZE;
        } else if (ch == 13) { 
            system("cls");

            if (selected == 2) return; 

            if (products.empty()) {
                cout << "\n ‼️No products available!\n";
                system("pause");
                continue;
            }

            switch (selected) {
                case 0: viewAllProducts(); 
                break;
                case 1: productByCategory(); 
                break;
            }

            system("pause");
        }
    }
}

void ProductManagementSystem::searchProductMenu() {
    int currentOption = 0;
    const int numOptions = 4;
    string options[] = {
        "1️⃣   Search by Product ID",
        "2️⃣   Search by Product Name",
        "3️⃣   Search by Category UUID",
        "🅾️   Back to Dashboard"
    };

    while (true) {
        system("cls");
        cout << "\n=========== SEARCH PRODUCT ===========\n";

        for (int i = 0; i < numOptions; ++i) {
            if (i == currentOption)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }

        cout << "======================================\n";
        cout << "↕️   Use arrows and press Enter to select.\n";

        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 72 && currentOption > 0) currentOption--;        
            else if (key == 80 && currentOption < numOptions - 1) currentOption++; 
        } else if (key == 13) {  
            if (currentOption == 3) break;

            system("cls");
            string query;
            cout << "\nEnter search query: ";
            getline(cin, query);

            Table table;
            table.add_row({
                "No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                "Created", "Expiry", "TotalPrice", "Status"
            });

            int count = 1;
            for (const auto& product : products) {
                bool match = false;
                string categoryName = "Unknown";
                string displaySize = product.size.empty() ? "N/A" : product.size;

                for (const auto& category : categories) {
                    if (category.uuid == product.categoryUuid) {
                        categoryName = category.name;
                        break;
                    }
                }

                if (currentOption == 0 && product.id == query) match = true;
                if (currentOption == 1 && product.name.find(query) != string::npos) match = true;
                if (currentOption == 2 && product.categoryUuid == query) match = true;

                if (match) {
                    ostringstream basePriceStream, sellPriceStream, totalPriceStream;
                    basePriceStream << fixed << setprecision(2) << product.basePrice;
                    sellPriceStream << fixed << setprecision(2) << product.sellPrice;
                    totalPriceStream << fixed << setprecision(2) << product.totalPrice;

                    table.add_row({
                        to_string(count++),
                        product.id,
                        product.name,
                        categoryName,
                        displaySize,
                        basePriceStream.str(),
                        sellPriceStream.str(),
                        to_string(product.quantity),
                        product.creationDate,
                        product.expirationDate,
                        totalPriceStream.str(),
                        product.status
                    });
                }
            }

            if (table.size() == 1) {
                cout << "\n❌ No matching product found.\n";
            } else {
                table[0].format().font_style({FontStyle::bold});
                cout << table << endl;
            }

            system("pause");
        }
    }
}

void ProductManagementSystem::updateProduct() {
    string id;
    cout << "\nEnter product ID to update: ";
    getline(cin, id);
    
    for (auto& product : products) {
        if (product.id == id) {
            string name, categoryUuid, expiration, size;
            double basePrice, sellPrice;
            int quantity;
            
            cout << "Enter new name (or press Enter to keep current): ";
            getline(cin, name);
            if (!name.empty()) product.name = name;
            
            cout << "Enter new category UUID (or press Enter to keep current, available categories):\n";
            for (const auto& category : categories) {
                cout << category.uuid << "\t" << category.name << endl;
            }
            cout << "Category UUID: ";
            getline(cin, categoryUuid);
            
            if (!categoryUuid.empty()) {
                bool validCategory = false;
                for (const auto& category : categories) {
                    if (category.uuid == categoryUuid) {
                        validCategory = true;
                        break;
                    }
                }
                if (validCategory) product.categoryUuid = categoryUuid;
                else cout << "Invalid category UUID, keeping current.\n";
            }
            
            cout << "Enter new base price (or 0 to keep current): ";
            cin >> basePrice;
            cout << "Enter new sell price (or 0 to keep current): ";
            cin >> sellPrice;
            
            if (basePrice > 0) {
                if (sellPrice > 0 && basePrice > sellPrice) {
                    cout << "Error: Base price cannot exceed sell price!\n";
                    return;
                }
                product.basePrice = basePrice;
            }
            if (sellPrice > 0) product.sellPrice = sellPrice;
            
            cout << "Enter new quantity (or -1 to keep current): ";
            cin >> quantity;
            
            if (quantity >= 0) {
                product.quantity = quantity;
                product.totalPrice = product.sellPrice * quantity;
                product.status = quantity > 0 ? "Available" : "false";
            }
            
            cin.ignore();
            cout << "Enter new expiration date (or press Enter to keep current): ";
            getline(cin, expiration);
            if (!expiration.empty()) product.expirationDate = expiration;
            
            cout << "Enter new size (or press Enter to keep current [" << (product.size.empty() ? "N/A" : product.size) << "]): ";
            getline(cin, size);
            if (!size.empty()) product.size = size;
            
            saveProducts();
            cout << "Product updated successfully!\n";
            return;
        }
    }
    cout << "Product not found!\n";
}

void ProductManagementSystem::deleteProduct() {
    if (currentUser->role != "Admin") {
        cout << "Only admins can delete products!\n";
        return;
    }
    
    string id;
    cout << "\nEnter product ID to delete: ";
    getline(cin, id);
    
    auto it = remove_if(products.begin(), products.end(),
        [&id](const Product& p) { return p.id == id; });
    
    if (it != products.end()) {
        products.erase(it, products.end());
        saveProducts();
        cout << "Product deleted successfully!\n";
    } else {
        cout << "Product not found!\n";
    }
}

void ProductManagementSystem::viewHighStockProducts() {
    const int ITEMS_PER_PAGE = 10;
    vector<Product> highStockProducts;
    for (const auto& product : products) {
        if (product.quantity >= 50) {
            highStockProducts.push_back(product);
        }
    }

    if (highStockProducts.empty()) {
        cout << "\n‼️ No high stock products found!\n";
        return;
    }

    int totalPages = (highStockProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    bool browsing = true;

    while (browsing) {
        system("cls");
        cout << "\n========== HIGH STOCK PRODUCTS (Page " << currentPage + 1 << " of " << totalPages << ") ==========\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        int startIdx = currentPage * ITEMS_PER_PAGE;
        int endIdx = min(startIdx + ITEMS_PER_PAGE, (int)highStockProducts.size());

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& product = highStockProducts[i];
            string categoryName = "Unknown";
            string displaySize = product.size.empty() ? "N/A" : product.size;

            for (const auto& category : categories) {
                if (category.uuid == product.categoryUuid) {
                    categoryName = category.name;
                    break;
                }
            }

            ostringstream basePriceStream, sellPriceStream, totalPriceStream;
            basePriceStream << fixed << setprecision(2) << product.basePrice;
            sellPriceStream << fixed << setprecision(2) << product.sellPrice;
            totalPriceStream << fixed << setprecision(2) << product.totalPrice;

            table.add_row({
                to_string(i + 1),
                product.id,
                product.name,
                categoryName,
                displaySize,
                basePriceStream.str(),
                sellPriceStream.str(),
                to_string(product.quantity),
                product.creationDate,
                product.expirationDate,
                totalPriceStream.str(),
                product.status
            });
        }

        table[0].format().font_style({FontStyle::bold});
        cout << table << endl;
        cout << "[⬅️  ] Previous | [➡️  ] Next | [ESC] Back";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 75) { 
                currentPage = (currentPage - 1 + totalPages) % totalPages;
            } else if (key == 77) { 
                currentPage = (currentPage + 1) % totalPages;
            }
        } else if (key == 27) {
            browsing = false;
        }
    }
}

void ProductManagementSystem::viewLowStockProducts() {
    const int ITEMS_PER_PAGE = 10;
    vector<Product> lowStockProducts;
    for (const auto& product : products) {
        if (product.quantity < 10) {
            lowStockProducts.push_back(product);
        }
    }

    if (lowStockProducts.empty()) {
        cout << "\n‼️ No low stock products found!\n";
        return;
    }

    int totalPages = (lowStockProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    bool browsing = true;

    while (browsing) {
        system("cls");
        cout << "\n========== LOW STOCK PRODUCTS (Page " << currentPage + 1 << " of " << totalPages << ") ==========\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        int startIdx = currentPage * ITEMS_PER_PAGE;
        int endIdx = min(startIdx + ITEMS_PER_PAGE, (int)lowStockProducts.size());

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& product = lowStockProducts[i];
            string categoryName = "Unknown";
            string displaySize = product.size.empty() ? "N/A" : product.size;

            for (const auto& category : categories) {
                if (category.uuid == product.categoryUuid) {
                    categoryName = category.name;
                    break;
                }
            }

            ostringstream basePriceStream, sellPriceStream, totalPriceStream;
            basePriceStream << fixed << setprecision(2) << product.basePrice;
            sellPriceStream << fixed << setprecision(2) << product.sellPrice;
            totalPriceStream << fixed << setprecision(2) << product.totalPrice;

            table.add_row({
                to_string(i + 1),
                product.id,
                product.name,
                categoryName,
                displaySize,
                basePriceStream.str(),
                sellPriceStream.str(),
                to_string(product.quantity),
                product.creationDate,
                product.expirationDate,
                totalPriceStream.str(),
                product.status
            });
        }

        table[0].format().font_style({FontStyle::bold});
        cout << table << endl;
        cout << "[⬅️  ] Previous | [➡️  ] Next | [ESC] Back\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 75) { 
                currentPage = (currentPage - 1 + totalPages) % totalPages;
            } else if (key == 77) { 
                currentPage = (currentPage + 1) % totalPages;
            }
        } else if (key == 27) { 
            browsing = false;
        }
    }
}



void ProductManagementSystem::sortProductsMenu() {
    int selected = 0;
    const int options = 13;
    const int PAGE_SIZE = 10;

    while (true) {
        system("cls");
        cout << "\n=========== SORT PRODUCTS ===========\n";

        string menu[] = {
            "1️⃣   By Name (A–Z)",
            "2️⃣   By Name (Z–A)",
            "3️⃣   By Base Price (Low to High)",
            "4️⃣   By Base Price (High to Low)",
            "5️⃣   By Sell Price (Low to High)",
            "6️⃣   By Sell Price (High to Low)",
            "7️⃣   By Quantity (Low to High)",
            "8️⃣   By Quantity (High to Low)",
            "9️⃣   By Creation Date (Newest First)",
            "🔟  By Creation Date (Oldest First)",
            "⏹️   By Total Price (Low to High)",
            "⏺️   By Total Price (High to Low)",
            "🅾️   Back to Dashboard"
        };

        for (int i = 0; i < options; i++) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }

        cout << "=====================================\n";
        cout << "↕️   Use arrow and press Enter to select.";

        int ch = _getch();
        if (ch == 224) {
            switch (_getch()) {
                case 72: selected = (selected - 1 + options) % options; break;  
                case 80: selected = (selected + 1) % options; break;            
            }
        } else if (ch == 13) { 
            if (selected == 12) break;

            vector<Product> sortedProducts = products;

            switch (selected) {
                case 0: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.name < b.name; }); break;
                case 1: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.name > b.name; }); break;
                case 2: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.basePrice < b.basePrice; }); break;
                case 3: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.basePrice > b.basePrice; }); break;
                case 4: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.sellPrice < b.sellPrice; }); break;
                case 5: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.sellPrice > b.sellPrice; }); break;
                case 6: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.quantity < b.quantity; }); break;
                case 7: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.quantity > b.quantity; }); break;
                case 8: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.creationDate > b.creationDate; }); break;
                case 9: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.creationDate < b.creationDate; }); break;
                case 10: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.totalPrice < b.totalPrice; }); break;
                case 11: sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) { return a.totalPrice > b.totalPrice; }); break;
            }

            int totalPages = (sortedProducts.size() + PAGE_SIZE - 1) / PAGE_SIZE;
            int currentPage = 0;

            while (true) {
                system("cls");
                Table table;
                table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty", 
                               "Created", "Expiry",  "TotalPrice", "Status"});
                table[0].format().font_style({FontStyle::bold});

                int start = currentPage * PAGE_SIZE;
                int end = min(start + PAGE_SIZE, (int)sortedProducts.size());

                for (int i = start; i < end; ++i) {
                    const Product& product = sortedProducts[i];

                    string categoryName = "Unknown";
                    for (const auto& category : categories) {
                        if (category.uuid == product.categoryUuid) {
                            categoryName = category.name;
                            break;
                        }
                    }

                    ostringstream basePriceStream, sellPriceStream, totalPriceStream;
                    basePriceStream << fixed << setprecision(2) << product.basePrice;
                    sellPriceStream << fixed << setprecision(2) << product.sellPrice;
                    totalPriceStream << fixed << setprecision(2) << product.totalPrice;

                    table.add_row({
                        to_string(i + 1),
                        product.id,
                        product.name,
                        categoryName,
                        product.size.empty() ? "N/A" : product.size,
                        basePriceStream.str(),
                        sellPriceStream.str(),
                        to_string(product.quantity),
                        product.creationDate,
                        product.expirationDate,
                        totalPriceStream.str(),
                        product.status
                    });
                }

                cout << table << "\n";
                cout << "Page " << (currentPage + 1) << " of " << totalPages << "\n";
                cout << "[⬅️  ] Prev Page  [➡️ ] Next Page  [ESC] Back";

                int key = _getch();
                if (key == 224) {
                    key = _getch();
                    if (key == 75 && currentPage > 0) currentPage--;         
                    else if (key == 77 && currentPage < totalPages - 1) currentPage++; 
                } else if (key == 27) {
                    break;  
                }
            }
        }
    }
}

void ProductManagementSystem::viewExpiredProducts() {
    const int ITEMS_PER_PAGE = 10;
    const string options[3] = {
        "1️⃣   Delete Expired Product",
        "2️⃣   Export Report",
        "🅾️   Back to Dashboard"
    };

    vector<Product> expiredProducts;
    for (const auto& product : products) {
        if (isExpired(product)) {
            expiredProducts.push_back(product);
        }
    }

    if (expiredProducts.empty()) {
        cout << "\n‼️ No expired products found.\n";
        presskeyEnter();
        return;
    }

    int totalPages = (expiredProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    int selected = 0;
    bool choosing = true;

    while (choosing) {
        system("cls");
        cout << "\n========== EXPIRED PRODUCTS (Page " << currentPage + 1 << " of " << totalPages << ") ==========\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Size", "Qty", "Created", "Expiry", "TotalPrice", "Status"});

        int startIdx = currentPage * ITEMS_PER_PAGE;
        int endIdx = min(startIdx + ITEMS_PER_PAGE, (int)expiredProducts.size());

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& product = expiredProducts[i];
            string displaySize = product.size.empty() ? "N/A" : product.size;
            ostringstream priceStream;
            priceStream << fixed << setprecision(2) << product.totalPrice;

            table.add_row({
                to_string(i + 1),
                product.id,
                product.name,
                displaySize,
                to_string(product.quantity),
                product.creationDate,
                product.expirationDate,
                priceStream.str(),
                product.status
            });
        }

        table[0].format().font_style({FontStyle::bold});
        cout << table << endl;

        cout << "===========================================\n";
        for (int i = 0; i < 3; ++i) {
            if (i == selected) cout << " 👉 ";
            else cout << "    ";
            cout << options[i] << endl;
        }
        cout << "===========================================\n";
        cout << "Page " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "[⬅️  ] Previous | [➡️  ] Next | [↕️  ] Select | and Enter";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72: selected = (selected - 1 + 3) % 3; break; 
                case 80: selected = (selected + 1) % 3; break;     
                case 75: currentPage = (currentPage - 1 + totalPages) % totalPages; break; 
                case 77: currentPage = (currentPage + 1) % totalPages; break;             
            }
        } else if (key == 13) {
            choosing = false;
        }
    }

    if (selected == 0 && currentUser->role == "Admin") {
        products.erase(
            remove_if(products.begin(), products.end(),
                [this](const Product& p) { return isExpired(p); }),
            products.end());
        saveProducts();
        cout << "✅ Expired products deleted successfully!\n";
    } else if (selected == 1) {
        ofstream file("expired_products.csv");
        file << "ID,Name,Expiration,CreationDate,Quantity,TotalPrice,Status,Size\n";
        for (const auto& product : expiredProducts) {
            string escapedName = product.name;
            string escapedSize = product.size;
            replace(escapedName.begin(), escapedName.end(), ',', ';');
            replace(escapedSize.begin(), escapedSize.end(), ',', ';');

            file << "\"" << product.id << "\",\"" << escapedName << "\",\"" << product.expirationDate << "\",\""
                 << product.creationDate << "\"," << product.quantity << ","
                 << fixed << setprecision(2) << product.totalPrice << ",\"" << product.status << "\",\"" << escapedSize << "\"\n";
        }
        file.close();
        cout << "✅ Expired products report exported to expired_products.csv\n";
    }
}


void ProductManagementSystem::presskeyEnter(){
    cout << "Press Enter to countinue...";
    cin.ignore();
    cin.get();

}

void ProductManagementSystem::restockProduct() {
    int selected = 0;
    const string menuItems[] = {
        "1️⃣   View Unavailable Products (Qty = 0)",
        "2️⃣   Restock by Product ID",
        "3️⃣   View Restock Log",
        "🅾️   Back to Admin Dashboard"
    };
    const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);

    while (true) {
        while (true) {
            system("cls");
            cout << "\n=========== RESTOCK PRODUCTS ===========\n";
            for (int i = 0; i < menuSize; ++i) {
                if (i == selected)
                    cout << " 👉 ";
                else
                    cout << "    ";
                cout << menuItems[i] << "\n";
            }
            cout << "========================================\n";
            cout << "↕️  Use arrow keys to navigate";

            int key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == 72) selected = (selected - 1 + menuSize) % menuSize; 
                else if (key == 80) selected = (selected + 1) % menuSize;       
            } else if (key == 13) {
                break; 
            }
        }

        switch (selected) {
            case 0: { 
                system("cls");

            bool hasUnavailable = false;

            if (!hasUnavailable) {
                cout << "❌ No unavailable products!\n";
                cout << "Press any key to return...";
                _getch();
                break;
            }

            cout << "\n=========== UNAVAILABLE PRODUCTS ===========\n";
            cout << "ID\tName\tCategory\tQty\tStatus\tSize\n";
            for (const auto& product : products) {
                if (product.quantity == 0) {
                    hasUnavailable = true;
                    string categoryName = "Unknown";
                    string displaySize = product.size.empty() ? "N/A" : product.size;
                
                    for (const auto& category : categories) {
                        if (category.uuid == product.categoryUuid) {
                            categoryName = category.name;
                            break;
                        }
                    }

                    cout << product.id << "\t" << product.name << "\t" << categoryName << "\t"
                         << product.quantity << "\t" << product.status << "\t" << displaySize << endl;
                }
            }

            cout << "\nPress 'R' to restock a product, or press ESC to return.";

            char action = _getch();
            if (action == 27) { 
                break;
            } else if (action == 'R' || action == 'r') {
                cout << "\nEnter product ID to restock: ";
                string id;
                getline(cin, id);

                bool found = false;
            for (auto& product : products) {
                if (product.id == id) {
                    found = true;
                    cout << "Product: " << product.name << " (Current Qty: " << product.quantity << ")\n";
                    cout << "Enter quantity to add: ";
                    int qty;
                    cin >> qty;
                    cin.ignore();

                if (qty <= 0) {
                    cout << "‼️ Invalid quantity! Must be positive.\n";
                    break;
                }

                product.quantity += qty;
                product.totalPrice = product.sellPrice * product.quantity;
                product.status = (product.quantity > 0) ? "Available" : "false";

                restockLogs.emplace_back(product.id, product.name, qty, getCurrentDate());
                saveProducts();
                saveRestockLogs();

                cout << "✅ Product " << product.name << " restocked successfully! New Qty: " << product.quantity << endl;
                break;
                }
            }

                if (!found) {
                    cout << "❌ Product ID not found!\n";
                }
            }
                break;
            }

            case 1: { 
                system("cls");
                cout << "Enter product ID to restock (or press Enter to cancel): ";
                string id;
                getline(cin, id);

                if (id.empty()) {
                    cout << "Restocking cancelled.\n";
                    break;
                }

                bool found = false;
                for (auto& product : products) {
                    if (product.id == id) {
                        found = true;
                        cout << "Product: " << product.name << " (Current Qty: " << product.quantity << ")\n";
                        cout << "Enter quantity to add: ";
                        int qty;
                        cin >> qty;
                        cin.ignore();

                        if (qty <= 0) {
                            cout << "‼️ Invalid quantity! Must be positive.\n";
                            break;
                        }

                        product.quantity += qty;
                        product.totalPrice = product.sellPrice * product.quantity;
                        product.status = (product.quantity > 0) ? "Available" : "false";

                        restockLogs.emplace_back(product.id, product.name, qty, getCurrentDate());
                        saveProducts();
                        saveRestockLogs();

                        cout << "✅ Product " << product.name << " restocked successfully! New Qty: " << product.quantity << endl;
                        break;
                    }
                }

                if (!found) {
                    cout << "❌ Product ID not found!\n";
                }
                break;
            }

            case 2: { 
                system("cls");
                if (restockLogs.empty()) {
                    cout << "❌ No restock logs!\n";
                    cout << "\nPress any key to return...";
                    _getch();
                break;
                }   
                cout << "\n=========== RESTOCK LOG ===========\n";
                cout << "ProductID\tProductName\tQtyRestocked\tRestockDate\n";
                for (const auto& log : restockLogs) {
                    cout << log.productId << "\t" << log.productName << "\t"
                         << log.quantityRestocked << "\t" << log.restockDate << endl;
                }
                break;
            }

            case 3: 
                return;

            default:
                cout << "Invalid choice!\n";
        }

        _getch(); 
    }
}

void ProductManagementSystem::placeOrder() {
    Order order(generateOrderId(), "");
    cout << "\nEnter customer name: ";
    getline(cin, order.customerName);

    int choice = 0;
    const int MENU_SIZE = 5;
    string options[] = {
        "1️⃣   Add Product to Cart",
        "2️⃣   View Cart",
        "3️⃣   Update Cart",
        "4️⃣   Confirm Order",
        "🅾️   Back to Dashboard"
    };

    while (true) {
        system("cls");
        cout << "\n========== 🛒 PLACE ORDER ==========\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == choice)
                cout << "👉 " << options[i] << "\n";
            else
                cout << "   " << options[i] << "\n";
        }
        cout << "====================================\n";
        cout << "↕️  Use UP/DOWN arrow keys to navigate and ENTER to select.";

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) choice = (choice - 1 + MENU_SIZE) % MENU_SIZE;  
            else if (key == 80) choice = (choice + 1) % MENU_SIZE;         
        } else if (key == 13) {
            system("cls");

            switch (choice) {
                case 0: addtoCart(order); break;
                case 1: viewCarts(order); break;
                case 2: updateCart(order); break;
                case 3: confirmCart(order); break;
                case 4: return;
            }

            system("pause");
        }
    }
}


void ProductManagementSystem::addtoCart(Order& order) {
    cout << "\n🔍 Search & Select Product\n";
    string id;
    cout << "Enter product ID (or press Enter to cancel): ";
    getline(cin, id);

    if (id.empty()) {
        cout << "❌ Cancelled adding product.\n";
        return;
    }

    Product* selectedProduct = nullptr;
    for (auto& product : products) {
        if (product.id == id) {
            selectedProduct = &product;
            break;
        }
    }

    if (!selectedProduct || selectedProduct->status == "Unavailable") {
        cout << "❌ Product not found or is unavailable!\n";
        return;
    }

    cout << "🛒 Product: " << selectedProduct->name << " (Stock: " << selectedProduct->quantity << ")\n";
    cout << "Enter quantity to add: ";

    int qty;
    cin >> qty;
    cin.ignore();

    if (qty <= 0) {
        cout << "❌ Invalid quantity!\n";
        return;
    }

    if (qty <= selectedProduct->quantity) {
        order.products.emplace_back(*selectedProduct, qty);
        order.total += selectedProduct->sellPrice * qty;
        selectedProduct->quantity -= qty;
        selectedProduct->totalPrice = selectedProduct->sellPrice * selectedProduct->quantity;
        selectedProduct->status = (selectedProduct->quantity > 0) ? "Available" : "Unavailable";
        cout << "✅ Added " << selectedProduct->name << " x" << qty << " to cart.\n";
    } else {
        cout << "❌ Insufficient stock!\n";
    }
}


void ProductManagementSystem::viewCarts(Order& order) {
    if (order.products.empty()) {
        cout << "\n🛒 Cart is empty!\n";
        return;
    }

    cout << "\n========== 🧾 CART SUMMARY ==========\n";
    cout << "- Order ID: " << order.orderId << "\n- Customer: " << order.customerName << "\n- Products:\n";

    for (const auto& item : order.products) {
        cout << " . " << item.first.id << " | " << item.first.name 
             << " x " << item.second << " = $" 
             << fixed << setprecision(2) << (item.first.sellPrice * item.second) << endl;
    }

    cout << "- Total: $" << fixed << setprecision(2) << order.total << "\n";
    cout << "=====================================\n";
}


void ProductManagementSystem::updateCart(Order& order){
if (order.products.empty()) {
                    cout << "\nCart is empty! Add products first.\n";
                    return;
                }
                
                cout << "\n========== UPDATE CART ==========\n";
                cout << "Current Cart:\n";
                
                for (size_t i = 0; i < order.products.size(); ++i) {
                    const auto& item = order.products[i];
                    cout << i + 1 << ". " << item.first.id << "\t" << item.first.name 
                         << " x" << item.second << " = $" << fixed << setprecision(2) 
                         << (item.first.sellPrice * item.second) << endl;
                }
                
                cout << "Enter item number to update (1-" << order.products.size() << ", or 0 to cancel): ";
                int itemNum;
                cin >> itemNum;
                cin.ignore();
                
                if (itemNum == 0) {
                    cout << "Update cancelled.\n";
                    return;
                }
                
                if (itemNum < 1 || itemNum > static_cast<int>(order.products.size())) {
                    cout << "Invalid item number!\n";
                    return;
                }
                
                auto& item = order.products[itemNum - 1];
                cout << "Current: " << item.first.name << " x" << item.second << endl;
                cout << "1. Change Quantity\n";
                cout << "2. Remove Item\n";
                cout << "Enter choice: ";
                
                int updateChoice;
                cin >> updateChoice;
                cin.ignore();
                
                if (updateChoice == 1) {
                    cout << "Enter new quantity (or 0 to cancel): ";
                    int newQty;
                    cin >> newQty;
                    cin.ignore();
                    
                    if (newQty == 0) {
                        cout << "Quantity update cancelled.\n";
                        return;
                    }
                    
                    if (newQty < 0) {
                        cout << "Invalid quantity! Must be non-negative.\n";
                        return;
                    }
                    
                    for (auto& product : products) {
                        if (product.id == item.first.id) {
                            int stockChange = item.second - newQty; 
                            
                            if (stockChange < 0 && -stockChange > product.quantity) {
                                cout << "Insufficient stock! Available: " << product.quantity << endl;
                                return;
                            }
                            
                            product.quantity += stockChange;
                            product.totalPrice = product.sellPrice * product.quantity;
                            product.status = (product.quantity > 0) ? "Available" : "false";
                            
                            order.total -= item.first.sellPrice * item.second;
                            item.second = newQty;
                            order.total += item.first.sellPrice * newQty;
                            
                            cout << "Quantity updated to " << newQty << " for " << item.first.name << endl;
                            
                            if (newQty == 0) {
                                order.products.erase(order.products.begin() + (itemNum - 1));
                                cout << "Item removed from cart (quantity set to 0).\n";
                            }
                            break;
                        }
                    }
                } else if (updateChoice == 2) {
                    for (auto& product : products) {
                        if (product.id == item.first.id) {
                            product.quantity += item.second;
                            product.totalPrice = product.sellPrice * product.quantity;
                            product.status = (product.quantity > 0) ? "Available" : "false";
                            break;
                        }
                    }
                    
                    order.total -= item.first.sellPrice * item.second;
                    order.products.erase(order.products.begin() + (itemNum - 1));
                    cout << "Item removed from cart.\n";
                } else {
                    cout << "Invalid choice!\n";
                }
}

void ProductManagementSystem::confirmCart(Order& order) {
    if (order.products.empty()) {
        cout << "\n🛒 Cart is empty! Add products before confirming.\n";
        return;
    }

    cout << "\n========== ✅ FINAL CART ==========\n";
    cout << "- Order ID: " << order.orderId << "\n- Customer: " << order.customerName << "\n- Products:\n";

    for (const auto& item : order.products) {
        cout << "  . " << item.first.id << " | " << item.first.name 
             << " x " << item.second << " = $" 
             << fixed << setprecision(2) << (item.first.sellPrice * item.second) << endl;
    }

    cout << "- Total: $" << fixed << setprecision(2) << order.total << endl;
    cout << "Confirm order (y/n): ";

    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);

        stringstream ss;
        ss << put_time(&localTime, "%a %e %b %Y, %H:%M:%S");
        order.dateTime = ss.str();

        orders.push_back(order);
        saveProducts();
        saveOrders();

        cout << "✅ Order placed successfully!\n";
        cout << "- Order ID: " << order.orderId << "\n";
        cout << "- Time: " << order.dateTime << endl;
    } else {
        cout << "❌ Order confirmation cancelled.\n";
    }
}


void ProductManagementSystem::viewStaffOrders() {
    if (orders.empty()) {
        cout << "\n‼️ No orders have been placed yet.\n";
        return;
    }

    cout << endl;
    Table table;
    table.add_row({"No", "Order ID", "Customer Name", "Total ($)"});

    int index = 1;
    for (const auto& order : orders) {
        ostringstream totalStream;
        totalStream << fixed << setprecision(2) << order.total;

        table.add_row({
            to_string(index++),
            order.orderId,
            order.customerName,
            totalStream.str()
        });
    }

    table[0].format().font_style({FontStyle::bold});
    cout << "\n============ STAFF ORDERS ============\n";
    cout << table << endl;
    system("pause");
}

void ProductManagementSystem::generateReport() {
    const string options[] = {
        "1️⃣   Inventory Summary",
        "2️⃣   View Low Stock Products",
        "3️⃣   View High Stock Products",
        "4️⃣   Export Product Report",
        "5️⃣   Export All Data to CSV",
        "🅾️   Back to Admin Dashboard"
    };
    const int optionCount = sizeof(options) / sizeof(options[0]);
    
    int selected = 0;

    while (true) {
        system("cls");  
        cout << "\n============= REPORT DASHBOARD =============\n";

        for (int i = 0; i < optionCount; ++i) {
            if (i == selected){
                cout << " 👉 " << options[i] << "\n";  
            }else{
                cout << "    " << options[i] << "\n";
            }
        }

        cout << "===========================================\n";
        cout << "↕️   Use UP/DOWN arrow keys. Press ENTER to select.";

        int key = _getch();
        if (key == 224) { 
            key = _getch(); 
            if (key == 72 && selected > 0) selected--;        
            else if (key == 80 && selected < optionCount - 1) selected++; 
        } else if (key == 13) { 
            switch (selected) {
                case 0: {
                    double totalBaseValue = 0, totalSellValue = 0;
                    int totalQuantity = 0, lowStock = 0, unavailable = 0;
                    const Product* mostStocked = nullptr;
                    const Product* highestPriced = nullptr;

                    for (const auto& product : products) {
                        totalBaseValue += product.basePrice * product.quantity;
                        totalSellValue += product.totalPrice;
                        totalQuantity += product.quantity;
                        if (product.quantity < 10) lowStock++;
                        if (product.quantity == 0) unavailable++;
                        if (!mostStocked || product.quantity > mostStocked->quantity)
                            mostStocked = &product;
                        if (!highestPriced || product.sellPrice > highestPriced->sellPrice)
                            highestPriced = &product;
                    }

                    cout << "\n=============== INVENTORY SUMMARY =================\n";
                    cout << "- Total Products..................: " << products.size() << endl;
                    cout << "- Total Stock Quantity............: " << totalQuantity << endl;
                    cout << "- Total Base Value................: $" << fixed << setprecision(2) << totalBaseValue << endl;
                    cout << "- Total Sell Value................: $" << totalSellValue << endl;
                    cout << "- Low Stock Items.................: " << lowStock << endl;
                    cout << "- Unavailable Items...............: " << unavailable << endl;
                    cout << "- Most Stocked Product............: " << (mostStocked ? mostStocked->name : "None") << endl;
                    cout << "- Highest Priced Product..........: " << (highestPriced ? highestPriced->name : "None") << endl;
                    cout << "===================================================\n";
                    system("pause");
                    break;
                }
                case 1: 
                viewLowStockProducts(); 
                system("pause"); 
                break;
                case 2: {
                    cout << endl;
                    viewHighStockProducts();
                    presskeyEnter();
                    break;
                }
                case 3: exportProductReport(); 
                system("pause"); 
                break;
                case 4: 
                exportDataToCsv(); 
                system("pause");
                break;
                case 5: return; 
            }
        }
    }
}

void ProductManagementSystem::exportToCsv(){
     ofstream file("products_export.csv");
                file << "ID,Name,Category,BasePrice,SellPrice,Quantity,Expiration,CreationDate,TotalPrice,Status,Size\n";
                
                for (const auto& product : products) {
                    string categoryName = "Unknown";
                    for (const auto& category : categories) {
                        if (category.uuid == product.categoryUuid) {
                            categoryName = category.name;
                            break;
                        }
                    }
                    
                    string escapedName = product.name;
                    string escapedCategory = categoryName;
                    string escapedExpiration = product.expirationDate;
                    string escapedCreationDate = product.creationDate;
                    string escapedSize = product.size;
                    
                    replace(escapedName.begin(), escapedName.end(), ',', ';');
                    replace(escapedCategory.begin(), escapedCategory.end(), ',', ';');
                    replace(escapedExpiration.begin(), escapedExpiration.end(), ',', ';');
                    replace(escapedCreationDate.begin(), escapedCreationDate.end(), ',', ';');
                    replace(escapedSize.begin(), escapedSize.end(), ',', ';');
                    
                    file << "\"" << product.id << "\",\"" << escapedName << "\",\"" << escapedCategory << "\","
                         << fixed << setprecision(2) << product.basePrice << "," << product.sellPrice << ","
                         << product.quantity << ",\"" << escapedExpiration << "\",\"" << escapedCreationDate << "\","
                         << product.totalPrice << ",\"" << product.status << "\",\"" << escapedSize << "\"\n";
                }
                file.close();
                cout << "Report exported to products_export.csv\n";
}

void ProductManagementSystem::exportToTxt() {
    ofstream file("products_export.txt");
    file << "ID\tName\tCategory\tBasePrice\tSellPrice\tQuantity\tExpiration\tCreationDate\tTotalPrice\tStatus\tSize\n";
    
    for (const auto& product : products) {
        string categoryName = "Unknown";
        string displaySize = product.size.empty() ? "N/A" : product.size;
        
        for (const auto& category : categories) {
            if (category.uuid == product.categoryUuid) {
                categoryName = category.name;
                break;
            }
        }
        
        file << product.id << "\t" << product.name << "\t" << categoryName << "\t"
             << fixed << setprecision(2) << product.basePrice << "\t" << product.sellPrice << "\t"
             << product.quantity << "\t" << product.expirationDate << "\t" << product.creationDate << "\t"
             << product.totalPrice << "\t" << product.status << "\t" << displaySize << endl;
    }
    file.close();
    cout << "Report exported successfully to products_export.txt\n";
}

void ProductManagementSystem::exportProductReport() {
    const int MENU_SIZE = 4;
    string options[MENU_SIZE] = {
        "1️⃣   Export to CSV",
        "2️⃣   Export to TXT",
        "3️⃣   Print Report",
        "🅾️   Back to Dashboard"
    };

    int selected = 0;

    while (true) {
        system("cls");
        cout << "\n========== EXPORT PRODUCT REPORT ==========\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "===========================================\n";
        cout << "↕️   Use arrows to navigate, ENTER to select.";

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72)       selected = (selected - 1 + MENU_SIZE) % MENU_SIZE; 
            else if (ch == 80)  selected = (selected + 1) % MENU_SIZE;             
        } else if (ch == 13) { 
            system("cls");
            switch (selected) {
                case 0:
                    exportToCsv();
                break;
                case 1: 
                     exportToTxt(); break;
                case 2: 
                    viewAllProducts(); 
                    presskeyEnter();
                    break; 
                case 3: return; 
            }
        }
    }
}

void ProductManagementSystem::exportDataToCsv() {
    ofstream productFile("products_report.csv");
    productFile << "ID,Name,Category,BasePrice,SellPrice,Quantity,Expiration,CreationDate,TotalPrice,Status,Size\n";
    
    for (const auto& product : products) {
        string categoryName = "Unknown";
        for (const auto& category : categories) {
            if (category.uuid == product.categoryUuid) {
                categoryName = category.name;
                break;
            }
        }
        
        string escapedName = product.name;
        string escapedCategory = categoryName;
        string escapedExpiration = product.expirationDate;
        string escapedCreationDate = product.creationDate;
        string escapedSize = product.size;
        
        replace(escapedName.begin(), escapedName.end(), ',', ';');
        replace(escapedCategory.begin(), escapedCategory.end(), ',', ';');
        replace(escapedExpiration.begin(), escapedExpiration.end(), ',', ';');
        replace(escapedCreationDate.begin(), escapedCreationDate.end(), ',', ';');
        replace(escapedSize.begin(), escapedSize.end(), ',', ';');
        
        productFile << "\"" << product.id << "\",\"" << escapedName << "\",\"" << escapedCategory << "\","
                    << fixed << setprecision(2) << product.basePrice << "," << product.sellPrice << ","
                    << product.quantity << ",\"" << escapedExpiration << "\",\"" << escapedCreationDate << "\","
                    << product.totalPrice << ",\"" << product.status << "\",\"" << escapedSize << "\"\n";
    }
    productFile.close();

    ofstream categoryFile("categories_report.csv");
    categoryFile << "UUID,Name\n";
    for (const auto& category : categories) {
        string escapedName = category.name;
        replace(escapedName.begin(), escapedName.end(), ',', ';');
        categoryFile << "\"" << category.uuid << "\",\"" << escapedName << "\"\n";
    }
    categoryFile.close();

    ofstream userFile("users_report.csv");
    userFile << "Username,Role\n";
    for (const auto& user : users) {
        string escapedUsername = user.username;
        replace(escapedUsername.begin(), escapedUsername.end(), ',', ';');
        userFile << "\"" << escapedUsername << "\",\"" << user.role << "\"\n";
    }
    userFile.close();

    ofstream orderFile("orders_report.csv");
    orderFile << "OrderID,CustomerName,Total,Products\n";
    for (const auto& order : orders) {
        string escapedCustomerName = order.customerName;
        replace(escapedCustomerName.begin(), escapedCustomerName.end(), ',', ';');
        
        string productsStr;
        for (size_t i = 0; i < order.products.size(); ++i) {
            productsStr += order.products[i].first.id + ":" + to_string(order.products[i].second);
            if (i < order.products.size() - 1) productsStr += ";";
        }
        
        orderFile << "\"" << order.orderId << "\",\"" << escapedCustomerName << "\","
                  << fixed << setprecision(2) << order.total << ",\"" << productsStr << "\"\n";
    }
    orderFile.close();

    ofstream restockFile("restock_report.csv");
    restockFile << "ProductID,ProductName,QuantityRestocked,RestockDate\n";
    for (const auto& log : restockLogs) {
        string escapedProductName = log.productName;
        replace(escapedProductName.begin(), escapedProductName.end(), ',', ';');
        restockFile << "\"" << log.productId << "\",\"" << escapedProductName << "\","
                    << log.quantityRestocked << ",\"" << log.restockDate << "\"\n";
    }
    restockFile.close();

    cout << "Data exported successfully to:\n";
    cout << "- products_report.csv\n";
    cout << "- categories_report.csv\n";
    cout << "- users_report.csv\n";
    cout << "- orders_report.csv\n";
    cout << "- restock_report.csv\n";
}
