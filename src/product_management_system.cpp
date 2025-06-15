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
#include <thread>
#include <chrono>
#include <filesystem>
#include <string>
#include <iostream>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"

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

        printLogo();
        cout << "+============================================+\n";
        cout << "|          PRODUCT MANAGEMENT SYSTEM         |\n";
        cout << "+============================================+\n";

        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }

        cout << "==============================================\n";
        cout << "↕️   Use arrow keys to navigate.";

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
        cout << "\n\n+==========================================+"<<endl;
        cout << "|                   LOGIN                  |" <<endl;
        cout << "+==========================================+"<<endl;
        username = getHiddenInput(" - Enter Username: ");
        password = getHiddenInput(" - Enter Password: ");
        cout << "+==========================================+"<<endl;

        for (auto& user : users) {
            if (user.username == username && user.password == password && user.role == role) {
                currentUser = &user;
                cout << "  ✅  Login successful!\n";
                return true;
            }
        }

        attempts--;
        cout << " ‼️  Invalid credentials! " << attempts << " attempts remaining.\n";
    }

    cout << " 🔂  Max login attempts reached!\n";
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
        cout << "\n\n+=========================================+\n";
        cout << "|              ADMIN DASHBOARD            |\n";
        cout << "+=========================================+\n";
        
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }

        cout << "+=========================================+\n";
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
        cout << "\n\n+======================================+\n";
        cout << "|            PRODUCT MANAGEMENT        |\n";
        cout << "+======================================+\n";
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }
        cout << "+======================================+\n";
        cout << "↕️  Use arrow to navigate.";

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
        "1️⃣   Product Management",
        "2️⃣   Place an Order for the Customer",
        "3️⃣   View Orders Placed",
        "🅾️   Logout"
    };

    while (true) {
        system("cls");
        cout << "\n\n+=========================================+\n";
        cout << "|               STAFF DASHBOARD           |\n";
        cout << "+=========================================+\n";
        for (int i = 0; i < options; ++i) {
            if (i == selected)
                cout << "👉 " << menu[i] << "\n";
            else
                cout << "   " << menu[i] << "\n";
        }
        cout << "+=========================================+\n";
        cout << "↕️   Use arrow to navigate.";

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
    getline(file, line); // Skip header

    while (getline(file, line)) {
        vector<string> fields = parseCsvLine(line);
        if (fields.size() >= 5) {
            try {
                string orderId = fields[0];
                string customerName = fields[1];
                string productsStr = fields[2];
                double total = stod(fields[3]);
                string dateTime = fields[4];

                Order order(orderId, customerName);
                order.total = total;
                order.dateTime = dateTime;

                stringstream ss(productsStr);
                string productEntry;

                while (getline(ss, productEntry, ';')) {
                    if (productEntry.empty()) continue;
                    size_t pos = productEntry.find(':');
                    if (pos == string::npos) continue;

                    string productName = productEntry.substr(0, pos);
                    int qty = stoi(productEntry.substr(pos + 1));
                    bool found = false;

                    for (const auto& p : products) {
                        if (p.name == productName) {
                            order.products.emplace_back(p, qty);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        cout << "Warning: Product '" << productName
                             << "' not found in order " << order.orderId << endl;
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
    if (!file.is_open()) {
        cerr << "Error: Unable to open " << ORDER_FILE << " for writing.\n";
        return;
    }

    file << "OrderID,CustomerName,Products,Total,Date\n";

    for (const auto& order : orders) {
        string escapedCustomerName = order.customerName;
        replace(escapedCustomerName.begin(), escapedCustomerName.end(), ',', ';');

        string productsStr;
        for (size_t i = 0; i < order.products.size(); ++i) {
            string name = order.products[i].first.name;
            replace(name.begin(), name.end(), ';', '_');
            productsStr += name + ":" + to_string(order.products[i].second);
            if (i < order.products.size() - 1) productsStr += ";";
        }

        file << "\"" << order.orderId << "\","
             << "\"" << escapedCustomerName << "\","
             << "\"" << productsStr << "\","
             << "\"" << fixed << setprecision(2) << order.total << "\","
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

void ProductManagementSystem::addUser() {
    system("cls");
    cout << endl;
    cout << "+-------------------------------------+" << endl;
    cout << "|               Add User              |" << endl;
    cout << "+-------------------------------------+\n";

    while (true) {
        string username, password, role;
        while (true) {
            username = inputNonEmptyString(" - Enter Username: ");
            bool usernameExists = false;
            for (const auto& user : users) {
                if (user.username == username) {
                    usernameExists = true;
                    break;
                }
            }
            if (usernameExists) {
                cout << "  ‼️ Username already exists! Please choose a different username.\n";
            } else {
                break;
            }
        }

        password = inputNonEmptyString(" - Enter Password: ");

        while (true) {
            role = inputNonEmptyString(" - Enter role (Admin/Staff): ");
            if (role == "Admin" || role == "Staff") {
                break;
            } else {
                cout << "  ‼️ Invalid role! Must be 'Admin' or 'Staff'. Please try again.\n";
            }
        }

        users.emplace_back(username, password, role);
        saveUsers();
        cout << "   ✅  User added successfully!\n";

        string continueChoice;
        while (true) {
            cout << "   🅰️  Add another user? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n"; 
                break; 
            } else if (continueChoice == "n" || continueChoice == "N") {
                return; 
            } else {
                cout << "  ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}
void ProductManagementSystem::updateUser() {
    system("cls");
    cout << "+-------------------------------------+" << endl;
    cout << "|             Update User             |" << endl;
    cout << "+-------------------------------------+\n";

    if (users.empty()) {
        cout << "   ❌  No users available to update!\n";
        presskeyEnter();
        return;
    }

    while (true) {
        string username = inputWithCancel(" - Enter username to update: ");
        if (username.empty()) {
            cout << " 🔙  Update cancelled.\n";
            presskeyEnter();
            return;
        }

        bool found = false;
        for (auto& user : users) {
            if (user.username == username) {
                found = true;

                string password;
                cout << " - Enter new password (" << maskPassword(user.password) << "): ";
                getline(cin, password);

                if (!password.empty()) {
                    while (password.length() < 3) {
                        cout << "   ‼️ Password must be at least 3 characters! Try again.\n";
                        cout << " - Enter new password (" << maskPassword(user.password) << "): ";
                        getline(cin, password);
                        if (password.empty()) break;
                    }

                    if (!password.empty()) {
                        user.password = password;
                    }
                }

                string role;
                while (true) {
                    cout << " - Enter new role (Admin/Staff, [" << user.role << "]): ";
                    getline(cin, role);

                    if (role.empty()) break;

                    if (role == "Admin" || role == "Staff") {
                        user.role = role;
                        break;
                    } else {
                        cout << "   ‼️ Invalid role! Must be 'Admin' or 'Staff'.\n";
                    }
                }

                saveUsers();
                cout << "   ✅  User updated successfully!\n";
                break;
            }
        }

        if (!found) {
            cout << "   ❌  User not found!\n";
        }

        string continueChoice;
        while (true) {
            cout << "   🅰️  Update another user? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                std::cout << "\n";
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                return;
            } else {
                cout << "   ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::deleteUser() {
    system("cls");
    cout << endl;
    cout << "+-------------------------------------+" << endl;
    cout << "|             Delete User             |" << endl;
    cout << "+-------------------------------------+\n";

    if (users.empty()) {
        cout << "   ❌  No users available to delete!\n";
        presskeyEnter();
        return;
    }

    while (true) {
        string username = inputWithCancel(" - Enter username to delete: ");
        if (username.empty()) {
            cout << " 🔙  Deletion cancelled.\n";
            presskeyEnter();
            return;
        }

        auto it = remove_if(users.begin(), users.end(),
            [&username](const User& u) { return u.username == username; });

        if (it != users.end()) {
            users.erase(it, users.end());
            saveUsers();
            cout << "   ✅  User deleted successfully!\n";
        } else {
            cout << "   ❌  User not found!\n";
        }

        string continueChoice;
        while (true) {
            cout << "   🅰️  Delete another user? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                return;
            } else {
                cout << "   ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::viewAllUsers() {
    if (users.empty()) {
        cout << " ❌ No users found!\n";
        return;
    }

    const int pageSize = 5;
    int totalUsers = users.size();
    int totalPages = (totalUsers + pageSize - 1) / pageSize;
    int currentPage = 1;

    while (true) {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        cout << "\n";
        cout << "+------------------------------------+" << endl;
        cout << "|           🚹 User List             |" << endl;
        cout << "+------------------------------------+" << endl;

        Table table;
        table.add_row({"No", "Username", "Passwords", "Role"});
        table[0].format().font_style({FontStyle::bold});

        int startIdx = (currentPage - 1) * pageSize;
        int endIdx = min(startIdx + pageSize, totalUsers);
        for (int i = startIdx; i < endIdx; ++i) {
            string maskedPassword(users[i].password.length(), '*');
            table.add_row({
                to_string(i + 1),
                users[i].username,
                maskedPassword,
                users[i].role
            });
        }

        cout << table << endl;
        cout << "📕  Page " << currentPage << " of " << totalPages << "\n";
        cout << "[➡️  ] Next | [⬅️  ]Previous | [ESC] Quit\n";

        int key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 75) { 
                if (currentPage > 1) currentPage--;
            } else if (key == 77) { 
                if (currentPage < totalPages) currentPage++;
            }
        } else if (key == 27) { 
            cout << "✅ Exiting user list view.\n";
            break;
        }
    }
}

void ProductManagementSystem::manageUsers() {
    const int MENU_SIZE = 5;
    string options[MENU_SIZE] = {
        "1️⃣   Add New User",
        "2️⃣   View All Users",
        "3️⃣   Update User Role or Password",
        "4️⃣   Delete User",
        "🅾️   Back to Admin Dashboard"
    };

    int selected = 0;
    while (true) {
        system("cls"); 
        cout << "\n+=====================================+\n";
        cout << "|            USER MANAGEMENT          |\n";
        cout << "+=====================================+\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "=======================================\n";
        cout << "↕️  Use arrows to navigate.";

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
    system("cls");
    cout << "\n+-------------------------------------+" << endl;
    cout << "|             Add Category            |" << endl;
    cout << "+-------------------------------------+\n";

    while (true) {
        string name = inputNonEmptyString(" - Enter category name: ");

        bool duplicate = false;
        for (const auto& category : categories) {
            if (toLower(category.name) == toLower(name)) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            cout << " ‼️ Category with the name " << name << "\n   already exists! Please enter a different name.\n";
            continue;
        }

        string uuid = generateCategoryUuid();
        categories.emplace_back(uuid, name);
        saveCategories();
        cout << " ✅  Category added successfully! UUID: " << uuid << endl;

        string continueChoice;
        while (true) {
            cout << " - Add another category? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n";
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::updateCategory() {
    system("cls");
    cout << "\n+-------------------------------------+" << endl;
    cout << "|           Update Category           |" << endl;
    cout << "+-------------------------------------+\n";

    if (categories.empty()) {
        cout << " ❌  No categories available to update!\n";
        presskeyEnter();
        return;
    }

    while (true) {
        string uuid = inputWithCancel(" - Enter category UUID to update: ");
        if (uuid.empty()) {
            cout << " 🔙  Update cancelled.\n";
            presskeyEnter();
            return;
        }

        bool found = false;
        for (auto& category : categories) {
            if (category.uuid == uuid) {
                found = true;

                string newName = inputWithCancel(" - Enter new name", category.name);
                if (newName.empty()) {
                    cout << " 🔙  Update cancelled.\n";
                    break;
                }

                category.name = newName;
                saveCategories();
                cout << " ✅  Category updated successfully!\n";
                break;
            }
        }

        if (!found) {
            cout << " ❌  Category not found!\n";
        }

        string continueChoice;
        while (true) {
            cout << " - Update another category? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n";
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::viewAllCategories(int pageSize) {
    if (categories.empty()) {
        cout << "‼️ No categories found!\n";
        return;
    }

    int totalCategories = categories.size();
    int totalPages = (totalCategories + pageSize - 1) / pageSize;
    int currentPage = 1;

    while (true) {
        system("cls");

        cout << "\n";
        cout << "+------------------------------------+" <<endl;
        cout << "|         📋 Category List           |" <<endl;
        cout << "+------------------------------------+" <<endl;
        Table table;
        table.add_row({"No", "UUID", "Name"});
        table[0].format().font_style({FontStyle::bold});

        int startIdx = (currentPage - 1) * pageSize;
        int endIdx = std::min(startIdx + pageSize, totalCategories);

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& category = categories[i];
            table.add_row({
                to_string(i + 1),
                category.uuid,
                category.name
            });
        }

        cout << table << endl;
        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

        int ch = _getch();

        if (ch == 27) { 
            break;
        } else if (ch == 0 || ch == 224) {
            int arrow = _getch();
            if (arrow == 75) { 
                if (currentPage > 1) currentPage--;
            } else if (arrow == 77) { 
                if (currentPage < totalPages) currentPage++;
            }
        }
    }
}

void ProductManagementSystem::deleteCategory() {
    system("cls");
    cout << "\n+-------------------------------------+" << endl;
    cout << "|           Delete Category           |" << endl;
    cout << "+-------------------------------------+\n";

    if (categories.empty()) {
        cout << " ❌  No categories available to delete!\n";
        presskeyEnter();
        return;
    }

    while (true) {
        string uuid = inputWithCancel(" - Enter category UUID to delete: ");
        if (uuid.empty()) {
            cout << " 🔙  Deletion cancelled.\n";
            presskeyEnter();
            return;
        }

        auto it = remove_if(categories.begin(), categories.end(),
            [&uuid](const Category& c) { return c.uuid == uuid; });

        if (it != categories.end()) {
            categories.erase(it, categories.end());
            saveCategories();
            cout << " ✅  Category deleted successfully!\n";
        } else {
            cout << " ❌  Category not found!\n";
        }

        string continueChoice;
        while (true) {
            cout << " - Delete another category? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n";
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
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
        cout << "\n\n+=========================================+\n";
        cout << "|            CATEGORY MANAGEMENT          |\n";
        cout << "+=========================================+\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "===========================================\n";
        cout << " ↕️  Use arrows to navigate.";

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
        cout << "\n\n==========================================\n";
        cout << "|           PRODUCT MANAGEMENT           |\n";
        cout << "==========================================\n";
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

void ProductManagementSystem::addNewProduct() {
    system("cls");
    cout << "\n+-------------------------------------+" << endl;
    cout << "|             Add Product             |" << endl;
    cout << "+-------------------------------------+\n";

    while (true) {
        if (categories.empty()) {
            cout << " ‼️ No categories available! Please add a category first.\n";
            return;
        }

        system("cls");
        cout << " 📋  Available Categories:\n";
        cout << "+-------------------------------------+\n";
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << "  " << i + 1 << ". " << categories[i].name << " (UUID: " << categories[i].uuid << ")\n";
        }
        cout << "+-------------------------------------+\n";

        int categoryIndex = -1;
        while (true) {
            cout << " - Enter the number of the category (or 0 to cancel): ";
            if (!(cin >> categoryIndex)) {
                cout << " ‼️ Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (categoryIndex == 0) return;
            if (categoryIndex >= 1 && categoryIndex <= static_cast<int>(categories.size())) {
                break;
            }
            cout << " ‼️ Invalid number! Please try again.\n";
        }
        cin.ignore();

        string categoryUuid = categories[categoryIndex - 1].uuid;

        string id = generateProductId();
        string name, expiration, size;

        cout << " - Product ID: " << id << endl;

        name = inputNonEmptyString(" - Enter product name: ");
        double basePrice = inputNonNegativeDouble(" - Enter base price (cost): ");
        double sellPrice = inputSellPrice(basePrice);

        int quantity = inputPositiveInt(" - Enter quantity: ");
        expiration = inputExpireDate();

        cout << " - Enter size (press Enter to skip): ";
        getline(cin, size);

        string creationDate = getCurrentDate();
        double totalPrice = sellPrice * quantity;
        string status = quantity > 0 ? "Available" : "Unavailable";

        products.emplace_back(id, name, categoryUuid, basePrice, sellPrice, quantity,
                              expiration, creationDate, totalPrice, status, size);
        saveProducts();
        cout << " ✅ Product added successfully with ID: " << id << "!\n";

        char addMore;
        cout << " 🛄 Add another product? (Y/N): ";
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

    int totalPages = (products.size() + pageSize - 1) / pageSize; 
    int currentPage = 1;

    while (true) {
        system("cls"); 
        cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;
        cout << "|                                                     🛒 Product List                                                          |\n";
        cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        int start = (currentPage - 1) * pageSize;
        int end = min(start + pageSize, static_cast<int>(products.size()));
        int count = start + 1; 

        for (int i = start; i < end; ++i) {
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
        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

        int key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 75) { 
                if (currentPage > 1) currentPage--;
            } else if (key == 77) { 
                if (currentPage < totalPages) currentPage++;
            }
        } else if (key == 27) {
            break;
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
        cout << "\n‼️ No products found in this category!\n";
        return;
    }

    const int pageSize = 5;
    int totalPages = (filteredProducts.size() + pageSize - 1) / pageSize;
    int currentPage = 1;

    while (true) {
        system("cls");
        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;
        cout << "|                                                     🛒 Product List                                                          |\n";
        cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;
        cout << " 🛒 Products in Category: " << selectedCategoryName <<"\n";

        Table table;
        table.add_row({"No", "ID", "Name", "Category", "Size", "BasePrice", "SellPrice", "Qty",
                       "Created", "Expiry", "TotalPrice", "Status"});

        int start = (currentPage - 1) * pageSize;
        int end = min(start + pageSize, static_cast<int>(filteredProducts.size()));
        int count = start + 1;

        for (int i = start; i < end; ++i) {
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

        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 75 && currentPage > 1) {
                currentPage--;
            } else if (key == 77 && currentPage < totalPages) {
                currentPage++;
            }
        } else if (key == 27) { 
            break;
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
        cout << "+===================================+\n";
        cout << "|           VIEW PRODUCTS           |\n";
        cout << "+===================================+\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "=====================================\n";
        cout << "↕️  Use arrows to navigate";

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72)       selected = (selected - 1 + MENU_SIZE) % MENU_SIZE; 
            else if (ch == 80)  selected = (selected + 1) % MENU_SIZE;
        } else if (ch == 13) { 
            system("cls");

            if (selected == 2) return; 

            if (products.empty()) {
                cout << "\n‼️No products available!\n";
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
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|              Search Product              |" << endl;
    cout << "+==========================================+" << endl;
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
        cout << "\n+==========================================+" << endl;
        cout << "|              Search Product              |" << endl;
        cout << "+==========================================+" << endl;

        for (int i = 0; i < numOptions; ++i) {
            if (i == currentOption)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }

        cout << "======================================\n";
        cout << "↕️   Use arrows to navigate.\n";

        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 72 && currentOption > 0) currentOption--;        
            else if (key == 80 && currentOption < numOptions - 1) currentOption++; 
        } else if (key == 13) {  
            if (currentOption == 3) break;

            system("cls");
            cout << "\n+==========================================+" << endl;
            cout << "|              Search Product              |" << endl;
            cout << "+==========================================+" << endl;
            string query = inputNonEmptyString(" - Enter search query: ");

            string queryLower = query;
            transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

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

                string idLower = product.id;
                string nameLower = product.name;
                string categoryUuidLower = product.categoryUuid;
                transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
                transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                transform(categoryUuidLower.begin(), categoryUuidLower.end(), categoryUuidLower.begin(), ::tolower);

                if (currentOption == 0 && idLower == queryLower) match = true;
                if (currentOption == 1 && nameLower.find(queryLower) != string::npos) match = true;
                if (currentOption == 2 && categoryUuidLower == queryLower) match = true;

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
                cout << "\n ❌  No matching product found.\n";
            } else {
                table[0].format().font_style({FontStyle::bold});
                cout << table << endl;
            }

            presskeyEnter();
        }
    }
}

void ProductManagementSystem::updateProduct() {
    while (true) {
        system("cls");
        cout << "\n+==========================================+" << endl;
        cout << "|              Update Product              |" << endl;
        cout << "+==========================================+" << endl;

        string id = inputWithCancel(" - Enter product UUID to update: ");
        if (id.empty()) {
            cout << " 🔙 Update cancelled.\n";
            presskeyEnter();
            return;
        }

        bool found = false;
        for (auto& product : products) {
            if (product.id == id) {
                found = true;

                string name = inputOptionalString(" - Enter new name (" + product.name + "): ");
                if (!name.empty()) product.name = name;

                string categoryUuid;
                cout << " - Enter new category UUID (" << product.categoryUuid << "): ";
                getline(cin, categoryUuid);
                if (!categoryUuid.empty()) {
                    bool valid = false;
                    for (const auto& category : categories) {
                        if (category.uuid == categoryUuid) {
                            valid = true;
                            break;
                        }
                    }
                    if (valid) product.categoryUuid = categoryUuid;
                    else cout << " ‼️ Invalid category UUID. Keeping current.\n";
                }

                string input;
                while (true) {
                    cout << " - Enter new base price (" << fixed << setprecision(2) << product.basePrice << "): ";
                    getline(cin, input);
                    if (input.empty()) break;
                    try {
                        double basePrice = stod(input);
                        if (basePrice <= 0) throw runtime_error("non-positive");
                        product.basePrice = basePrice;
                        break;
                    } catch (...) {
                        cout << " ‼️ Invalid base price!\n";
                    }
                }

                while (true) {
                    cout << " - Enter new sell price (" << fixed << setprecision(2) << product.sellPrice << "): ";
                    getline(cin, input);
                    if (input.empty()) break;
                    try {
                        double sellPrice = stod(input);
                        if (sellPrice <= 0) throw runtime_error("non-positive");
                        if (sellPrice < product.basePrice) {
                            cout << " ⚠️ Sell price must not be less than base price!\n";
                        } else {
                            product.sellPrice = sellPrice;
                            break;
                        }
                    } catch (...) {
                        cout << " ‼️ Invalid sell price!\n";
                    }
                }

                while (true) {
                    cout << " - Enter new quantity (" << product.quantity << "): ";
                    getline(cin, input);
                    if (input.empty()) break;
                    try {
                        int quantity = stoi(input);
                        if (quantity < 0) throw runtime_error("negative");
                        product.quantity = quantity;
                        product.status = quantity > 0 ? "Available" : "Unavailable";
                        product.totalPrice = product.sellPrice * quantity;
                        break;
                    } catch (...) {
                        cout << " ‼️ Invalid quantity!\n";
                    }
                }

                cout << " - Enter new expiration date (" << product.expirationDate << "): ";
                getline(cin, input);
                if (!input.empty() && input.length() >= 3) product.expirationDate = input;

                cout << " - Enter new size (" << (product.size.empty() ? "N/A" : product.size) << "): ";
                getline(cin, input);
                if (!input.empty() && input.length() >= 3) product.size = input;

                saveProducts();
                cout << " ✅ Product updated successfully!\n";
                break;
            }
        }

        if (!found) {
            cout << " ❌ Product not found!\n";
        }

        string choice;
        while (true) {
            cout << " - Update another product? (y/n): ";
            getline(cin, choice);
            if (choice == "y" || choice == "Y") break;
            else if (choice == "n" || choice == "N") {
                presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::deleteProduct() {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|              Delete Product              |" << endl;
    cout << "+==========================================+" << endl;

    if (currentUser->role != "Admin") {
        cout << " ‼️ Only admins can delete products!\n";
        presskeyEnter();
        return;
    }

    if (products.empty()) {
        cout << " ❌ No products available to delete!\n";
        presskeyEnter();
        return;
    }

    while (true) {
        string id = inputWithCancel(" - Enter product ID to delete: ");

        if (id.empty()) {
            cout << " 🔁 Cancelled deletion.\n";
            //presskeyEnter();
            return;
        }

        auto it = remove_if(products.begin(), products.end(),
            [&id](const Product& p) { return p.id == id; });

        if (it != products.end()) {
            products.erase(it, products.end());
            saveProducts();
            cout << " ✅ Product deleted successfully!\n";
        } else {
            cout << " ❌ Product not found!\n";
        }

        string choice;
        while (true) {
            cout << " - Delete another product? (y/n): ";
            getline(cin, choice);
            if (choice == "y" || choice == "Y") {
                system("cls");
                cout << "\n+==========================================+" << endl;
                cout << "|              Delete Product              |" << endl;
                cout << "+==========================================+" << endl;
                break;
            } else if (choice == "n" || choice == "N") {
                presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
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
        cout << "\n  ‼️ No high stock products found!\n";
        return;
    }

    int totalPages = (highStockProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    bool browsing = true;

    while (browsing) {
        system("cls");
        cout << "+----------------------------------------------------------------------------------------------------------------------------+" <<endl;
        cout << "|                                                  💹 HIGHT STOCK PRODUCTS                                                  |\n";
        cout << "+----------------------------------------------------------------------------------------------------------------------------+" <<endl;

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
        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

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
        cout << "\n   ‼️ No low stock products found!\n";
        return;
    }

    int totalPages = (lowStockProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    bool browsing = true;

    while (browsing) {
        system("cls");
        cout << "+----------------------------------------------------------------------------------------------------------------------------+" <<endl;
        cout << "|                                                   💹 LOW STOCK PRODUCTS                                                    |\n";
        cout << "+----------------------------------------------------------------------------------------------------------------------------+" <<endl;
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
        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

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
        cout << "\n+===================================+\n";
        cout << "|            SORT PRODUCTS          |\n";
        cout << "+===================================+\n";

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
        cout << "↕️  Use arrow and press Enter to select.";

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
                cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;
                cout << "|                                                     💹 SORT PRODUCTS                                                         |\n";
                cout << "+------------------------------------------------------------------------------------------------------------------------------+" <<endl;
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
                cout << "📕  Page " << (currentPage + 1) << " of " << totalPages << "\n";
                cout << "----------------------------------------------------------------------------------------------------------------------------------" <<endl;
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
    // ✅ Restrict access to Admin only
    if (currentUser->role != "Admin") {
        cout << "\n  ❌ Access denied. Only Admin can view expired products.\n";
        presskeyEnter();
        return;
    }

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
        cout << "\n  ‼️ No expired products found.\n";
        presskeyEnter();
        return;
    }

    int totalPages = (expiredProducts.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    int currentPage = 0;
    int selected = 0;
    bool choosing = true;

    while (choosing) {
        system("cls");
        cout << "\n+----------------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "|                                                     EXPIRED PRODUCTS                                                       |\n";
        cout << "+----------------------------------------------------------------------------------------------------------------------------+" << endl;

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
        cout << "📕  Page " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "----------------------------------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < 3; ++i) {
            if (i == selected) cout << " 👉 ";
            else cout << "    ";
            cout << options[i] << endl;
        }
        cout << "----------------------------------------------------------------------------------------------------------------------------\n";
        cout << "[⬅️ ] Previous | [➡️ ] Next | [↕️ ] Select | and Enter";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72: selected = (selected - 1 + 3) % 3; break; // Up
                case 80: selected = (selected + 1) % 3; break;     // Down
                case 75: currentPage = (currentPage - 1 + totalPages) % totalPages; break; // Left
                case 77: currentPage = (currentPage + 1) % totalPages; break;             // Right
            }
        } else if (key == 13) {
            choosing = false;
        }
    }

    // ✅ Handle user selection
    if (selected == 0) {
        char confirm;
        cout << "\n  ❓ Are you sure you want to delete all expired products? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            products.erase(
                remove_if(products.begin(), products.end(),
                    [this](const Product& p) { return isExpired(p); }),
                products.end());
            saveProducts();
            cout << "   ✅ Expired products deleted successfully!\n";
        } else {
            cout << "   ❎ Deletion cancelled.\n";
        }
    } else if (selected == 1) {
        // Export to CSV
        ofstream file("expired_products.csv");
        file << "ID,Name,Expiration,CreationDate,Quantity,TotalPrice,Status,Size\n";
        for (const auto& product : expiredProducts) {
            string escapedName = product.name;
            string escapedSize = product.size;
            replace(escapedName.begin(), escapedName.end(), ',', ';');
            replace(escapedSize.begin(), escapedSize.end(), ',', ';');

            file << "\"" << product.id << "\",\"" << escapedName << "\",\"" << product.expirationDate << "\",\""
                 << product.creationDate << "\"," << product.quantity << "," 
                 << fixed << setprecision(2) << product.totalPrice << ",\"" 
                 << product.status << "\",\"" << escapedSize << "\"\n";
        }
        file.close();
        cout << "   ✅ Expired products report exported to expired_products.csv\n";
    }

    presskeyEnter(); // Pause before returning
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
            cout << "\n+==========================================+" << endl;
            cout << "|             Restock Products             |" << endl;
            cout << "+==========================================+" << endl;
            for (int i = 0; i < menuSize; ++i) {
                if (i == selected)
                    cout << " 👉 ";
                else
                    cout << "    ";
                cout << menuItems[i] << "\n";
            }
            cout << "+==========================================+\n";
            cout << "↕️  Use arrow keys to navigate\n";

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
                showUnavailableProducts();
                break;
            }
            case 1: { 
                restockProductById();
                break;
            }
            case 2: { 
                showRestockLogs();
                break;
            }
            case 3: 
                return;

            default:
                cout << " ‼️ Invalid choice!\n";
                presskeyEnter();
        }
    }
}

void ProductManagementSystem::placeOrder() {
    Order order(generateOrderId(), "");
    cout << "\n - Enter customer name: ";
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
        cout << "\n+==================================+\n";
        cout << "|         🛒 PLACE ORDER          |\n";
        cout << "+==================================+\n";
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == choice)
                cout << "👉 " << options[i] << "\n";
            else
                cout << "   " << options[i] << "\n";
        }
        cout << "====================================\n";
        cout << "↕️  Use arrow keys to navigate.";

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) choice = (choice - 1 + MENU_SIZE) % MENU_SIZE;  
            else if (key == 80) choice = (choice + 1) % MENU_SIZE;         
        } else if (key == 13) {
            switch (choice) {
                case 0: addtoCart(order); break;
                case 1: viewCarts(order); break;
                case 2: updateCart(order); break;
                case 3: confirmCart(order); 
                presskeyEnter();
                return;
                case 4: return;
            }

            system("pause");
        }
    }
}

void ProductManagementSystem::addtoCart(Order& order) {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|            Add Product to Cart            |" << endl;
    cout << "+==========================================+" << endl;

    while (true) {
        cout << "🔍 Search & Select Product\n";
        cout << " - Enter product ID or name: ";
        string query;
        getline(cin, query);

        if (query.empty()) {
            cout << "   ❌ Cancelled adding product.\n";
            break; 
        }

        while (!query.empty() && query.length() < 3) {
            cout << "  ‼️ Input must be at least 3 characters long! Please try again.\n";
            cout << " - Enter product ID or name (or press Enter to cancel): ";
            getline(cin, query);
            if (query.empty()) {
                cout << "   ❌ Cancelled adding product.\n";
                break;
            }
        }
        if (query.empty()) break;

        string queryLower = query;
        transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

        vector<Product*> matches;
        for (auto& product : products) {
            string idLower = product.id;
            string nameLower = product.name;
            transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            if (idLower == queryLower || nameLower.find(queryLower) != string::npos) {
                if (product.status != "Unavailable") {
                    matches.push_back(&product);
                }
            }
        }

        Product* selectedProduct = nullptr;
        if (matches.empty()) {
            cout << "   ❌ No available products found matching '" << query << "'!\n";
        } else if (matches.size() == 1) {
            selectedProduct = matches[0];
        } else {
            system("cls");
            cout << "\n+==========================================+" << endl;
            cout << "|         Multiple Products Found           |" << endl;
            cout << "+==========================================+" << endl;
            cout << "Multiple products match '" << query << "'. Please select one:\n\n";

            Table table;
            table.add_row({"No", "ID", "Name", "Stock", "Price"});
            table[0].format().font_style({FontStyle::bold});

            for (size_t i = 0; i < matches.size(); ++i) {
                table.add_row({
                    to_string(i + 1),
                    matches[i]->id,
                    matches[i]->name,
                    to_string(matches[i]->quantity),
                    to_string(matches[i]->sellPrice)
                });
            }

            cout << table << endl;
            string id = inputNonEmptyString(" - Enter product ID to select (or press Enter to cancel): ");
            if (id.empty()) {
                cout << "   ❌ Cancelled adding product.\n";
                break;
            }

            for (auto* product : matches) {
                if (product->id == id) {
                    selectedProduct = product;
                    break;
                }
            }
            if (!selectedProduct) {
                cout << "   ❌ Invalid product ID selected!\n";
            }
        }

        if (selectedProduct) {
            cout << "   🛒 Product: " << selectedProduct->name << " (Stock: " << selectedProduct->quantity << ", Price: $" << fixed << setprecision(2) << selectedProduct->sellPrice << ")\n";

            int qty;
            while (true) {
                cout << " - Enter quantity to add: ";
                string input;
                getline(cin, input);
                try {
                    qty = stoi(input);
                    if (qty <= 0) {
                        cout << "  ‼️ Invalid quantity! Must be positive.\n";
                        continue;
                    }
                    break;
                } catch (...) {
                    cout << "  ‼️ Invalid input! Please enter a valid integer.\n";
                }
            }

            if (qty <= selectedProduct->quantity) {
                order.products.emplace_back(*selectedProduct, qty);
                order.total += selectedProduct->sellPrice * qty;
                selectedProduct->quantity -= qty;
                selectedProduct->totalPrice = selectedProduct->sellPrice * selectedProduct->quantity;
                selectedProduct->status = (selectedProduct->quantity > 0) ? "Available" : "Unavailable";
                cout << "   ✅ Added " << selectedProduct->name << " x" << qty << " to cart.\n";
            } else {
                cout << "   ❌ Insufficient stock!\n";
            }
        }

        string continueChoice;
        while (true) {
            cout << " - Add another product to cart? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n";
                system("cls");
                cout << "\n+==========================================+" << endl;
                cout << "|            Add Product to Cart            |" << endl;
                cout << "+==========================================+" << endl;
                break; 
            } else if (continueChoice == "n" || continueChoice == "N") {
                return; 
            } else {
                cout << "   ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }

    string continueChoice;
    while (true) {
        cout << " - Add another product to cart? (y/n): ";
        getline(cin, continueChoice);
        if (continueChoice == "y" || continueChoice == "Y") {
            cout << "\n";
            system("cls");
            cout << "\n+==========================================+" << endl;
            cout << "|            Add Product to Cart            |" << endl;
            cout << "+==========================================+" << endl;
            continue; 
        } else if (continueChoice == "n" || continueChoice == "N") {
            return; 
        } else {
            cout << "   ‼️ Invalid input! Please enter 'y' or 'n'.\n";
        }
    }
}

void ProductManagementSystem::viewCarts(Order& order) {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|              Cart Summary                |" << endl;
    cout << "+==========================================+" << endl;

    if (order.products.empty()) {
        cout << " ❌ Cart is empty!\n";
        presskeyEnter();
        return;
    }

    cout << "🛒 Order ID: " << order.orderId << "\n";
    cout << "👤 Customer: " << order.customerName << "\n";
    cout << "📋 Products:\n";

    Table table;
    table.add_row({"No", "ID", "Name", "Size", "Category", "Qty", "Price", "Subtotal"});
    table[0].format().font_style({FontStyle::bold});

    int count = 1;
    for (const auto& item : order.products) {
        const auto& product = item.first;
        int quantity = item.second;
        string categoryName = "Unknown";
        string displaySize = product.size.empty() ? "N/A" : product.size;

        for (const auto& category : categories) {
            if (category.uuid == product.categoryUuid) {
                categoryName = category.name;
                break;
            }
        }
        ostringstream priceStream, subtotalStream;
        priceStream << fixed << setprecision(2) << product.sellPrice;
        subtotalStream << fixed << setprecision(2) << (product.sellPrice * quantity);

        table.add_row({
            to_string(count++),
            product.id,
            product.name,
            displaySize,
            categoryName,
            to_string(quantity),
            "$" + priceStream.str(),
            "$" + subtotalStream.str()
        });
    }

    cout << table << endl;
    cout << "💰 Total: $" << fixed << setprecision(2) << order.total << "\n";
    cout << "======================================\n";
}

void ProductManagementSystem::updateCart(Order& order) {
    system("cls");
    cout << "\n============================================" << endl;
    cout << "|              Update Cart                 |" << endl;
    cout << "============================================" << endl;

    while (true) {
        if (order.products.empty()) {
            cout << " ❌ Cart is empty! Add products first.\n";
            presskeyEnter();
            return;
        }

        cout << "🛒 Current Cart:\n\n";
        Table table;
        table.add_row({"No", "ID", "Name", "Qty", "Subtotal"});
        table[0].format().font_style({FontStyle::bold});

        for (size_t i = 0; i < order.products.size(); ++i) {
            const auto& item = order.products[i];
            ostringstream subtotalStream;
            subtotalStream << fixed << setprecision(2) << (item.first.sellPrice * item.second);
            table.add_row({
                to_string(i + 1),
                item.first.id,
                item.first.name,
                to_string(item.second),
                "$" + subtotalStream.str()
            });
        }

        cout << table << endl;
        cout << "💰 Total: $" << fixed << setprecision(2) << order.total << "\n";

        int itemNum;
        while (true) {
            cout << " - Enter item number to update (1-" << order.products.size() << ", or 0 to cancel): ";
            string input;
            getline(cin, input);
            try {
                itemNum = input.empty() ? 0 : stoi(input);
                if (itemNum < 0 || itemNum > static_cast<int>(order.products.size())) {
                    cout << " ‼️ Invalid item number! Please enter a number between 0 and " << order.products.size() << ".\n";
                    continue;
                }
                break;
            } catch (...) {
                cout << " ‼️ Invalid input! Please enter a valid number.\n";
            }
        }

        if (itemNum == 0) {
            cout << " ❌ Update cancelled.\n";
        } else {
            auto& item = order.products[itemNum - 1];
            int selected = 0;
            const string menuItems[] = {
                "1️⃣  Change Quantity",
                "2️⃣  Remove Item"
            };
            const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);

            bool cancelled = false;
            while (true) {
                system("cls");
                cout << "\n============================================" << endl;
                cout << "|              Update Item                 |" << endl;
                cout << "============================================" << endl;
                cout << "📋 Current: " << item.first.name << " x" << item.second << "\n\n";
                for (int i = 0; i < menuSize; ++i) {
                    if (i == selected)
                        cout << " 👉 ";
                    else
                        cout << "    ";
                    cout << menuItems[i] << "\n";
                }
                cout << "======================================\n";
                cout << "↕️  Use arrow keys to navigate, [ESC] to cancel\n";

                int key = _getch();
                if (key == 224) {
                    key = _getch();
                    if (key == 72) selected = (selected - 1 + menuSize) % menuSize; 
                    else if (key == 80) selected = (selected + 1) % menuSize;      
                } else if (key == 13) {
                    break; 
                } else if (key == 27) {
                    cancelled = true; 
                    break;
                }
            }

            if (cancelled) {
                cout << " ❌ Update cancelled.\n";
            } else {
                int updateChoice = selected + 1;
                if (updateChoice == 1) {
                    int newQty;
                    while (true) {
                        cout << " - Enter new quantity (or 0 to cancel): ";
                        string input;
                        getline(cin, input);
                        try {
                            newQty = input.empty() ? 0 : stoi(input);
                            if (newQty < 0) {
                                cout << " ‼️ Invalid quantity! Must be non-negative.\n";
                                continue;
                            }
                            break;
                        } catch (...) {
                            cout << " ‼️ Invalid input! Please enter a valid number.\n";
                        }
                    }

                    if (newQty == 0) {
                        cout << " ❌ Quantity update cancelled.\n";
                    } else {
                        for (auto& product : products) {
                            if (product.id == item.first.id) {
                                int stockChange = item.second - newQty;

                                if (stockChange < 0 && -stockChange > product.quantity) {
                                    cout << " ❌ Insufficient stock! Available: " << product.quantity << "\n";
                                    break;
                                }

                                product.quantity += stockChange;
                                product.totalPrice = product.sellPrice * product.quantity;
                                product.status = (product.quantity > 0) ? "Available" : "Unavailable";

                                order.total -= item.first.sellPrice * item.second;
                                item.second = newQty;
                                order.total += item.first.sellPrice * newQty;

                                cout << " ✅ Quantity updated to " << newQty << " for " << item.first.name << "\n";

                                if (newQty == 0) {
                                    order.products.erase(order.products.begin() + (itemNum - 1));
                                    cout << " ✅ Item removed from cart (quantity set to 0).\n";
                                }
                                break;
                            }
                        }
                    }
                } else if (updateChoice == 2) {
                    for (auto& product : products) {
                        if (product.id == item.first.id) {
                            product.quantity += item.second;
                            product.totalPrice = product.sellPrice * product.quantity;
                            product.status = (product.quantity > 0) ? "Available" : "Unavailable";
                            break;
                        }
                    }

                    order.total -= item.first.sellPrice * item.second;
                    order.products.erase(order.products.begin() + (itemNum - 1));
                    cout << " ✅ Item removed from cart.\n";
                }
            }
        }

        string continueChoice;
        while (true) {
            cout << " - Update another item? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "y" || continueChoice == "Y") {
                cout << "\n";
                system("cls");
                cout << "\n============================================" << endl;
                cout << "|              Update Cart                 |" << endl;
                cout << "============================================" << endl;
                break;
            } else if (continueChoice == "n" || continueChoice == "N") {
                //presskeyEnter();
                return;
            } else {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::confirmCart(Order& order) {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|              Confirm Cart                |" << endl;
    cout << "+==========================================+" << endl;

    if (order.products.empty()) {
        cout << " ❌ Cart is empty! Add products first.\n";
        presskeyEnter();
        return;
    }

    cout << "🛒 Order ID: " << order.orderId << "\n";
    cout << "👤 Customer: " << order.customerName << "\n";
    cout << "📋 Products:\n";

    Table table;
    table.add_row({"No", "ID", "Name", "Size", "Category", "Qty", "Price", "Subtotal"});
    table[0].format().font_style({FontStyle::bold});

    int count = 1;
    for (const auto& item : order.products) {
        const auto& product = item.first;
        int quantity = item.second;
        string categoryName = "Unknown";
        string displaySize = product.size.empty() ? "N/A" : product.size;

        for (const auto& category : categories) {
            if (category.uuid == product.categoryUuid) {
                categoryName = category.name;
                break;
            }
        }

        ostringstream priceStream, subtotalStream;
        priceStream << fixed << setprecision(2) << product.sellPrice;
        subtotalStream << fixed << setprecision(2) << (product.sellPrice * quantity);

        table.add_row({
            to_string(count++),
            product.id,
            product.name,
            displaySize,
            categoryName,
            to_string(quantity),
            "$" + priceStream.str(),
            "$" + subtotalStream.str()
        });
    }

    cout << table << endl;
    cout << "💰 Total: $" << fixed << setprecision(2) << order.total << "\n";

    string confirm;
    while (true) {
        cout << " - Confirm order? (y/n): ";
        getline(cin, confirm);
        if (confirm == "y" || confirm == "Y" || confirm == "n" || confirm == "N") {
            break;
        }
        cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (confirm == "y" || confirm == "Y") {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);

        stringstream ss;
        ss << put_time(&localTime, "%a %e %b %Y, %H:%M:%S");
        order.dateTime = ss.str();

        orders.push_back(order);
        saveProducts();
        saveOrders();

        // Print receipt to console
        cout << "\n+======================================================================+" << endl;
        cout << "|                                Receipt                               |" << endl;
        cout << "+======================================================================+" << endl;
        cout << "🛒 Order ID: " << order.orderId << "\n";
        cout << "👤 Customer: " << order.customerName << "\n";
        cout << "🕰️ Time: " << order.dateTime << "\n";
        cout << "📋 Products:\n";
        cout << table << endl;
        cout << "💰 Total: $" << fixed << setprecision(2) << order.total << "\n";
        cout << "+======================================================================+" << endl;
        cout << "| ✅ Thank you for your purchase!\n"; 
        cout << "+======================================================================+" << endl;

        // Save receipt to file
        string filename = "receipt_" + order.orderId + ".txt";
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << "+======================================================================+\n";
            outFile << "|                                 Receipt                              |\n";
            outFile << "+======================================================================+\n";
            outFile << "Order ID: " << order.orderId << "\n";
            outFile << "Customer: " << order.customerName << "\n";
            outFile << "Time: " << order.dateTime << "\n";
            outFile << "Products:\n";
            outFile << table << "\n";
            outFile << "Total: $" << fixed << setprecision(2) << order.total << "\n";
            outFile << "+======================================================================+\n";
            outFile << "Thank you for your purchase!\n";
            outFile << "+======================================================================+\n";
            outFile.close();
            cout << " 📄 Receipt saved to " << filename << "\n";
            cout << " 🗑️ Receipt will be deleted in 2 minutes.\n";

            // delete the file after 2 minutes
            thread([filename]() {
                this_thread::sleep_for(chrono::minutes(2));
                try {
                    if (filesystem::exists(filename)) {
                        filesystem::remove(filename);
                    }
                } catch (const filesystem::filesystem_error& e) {
                }
            }).detach();
        } else {
            cout << " ⚠️ Failed to save receipt to file.\n";
        }

        order.products.clear();
        order.total = 0.0;
        order.customerName = "";
        order.orderId = "";
        order.dateTime = "";
    } else {
        cout << " ❌ Order confirmation cancelled.\n";
    }

}

void ProductManagementSystem::displayOrderDetails(Order& order) {
    
    cout << "+-------------------------------------------------+" <<endl;
    cout << "|                   ORDER DETAILS                 |\n";
    cout << "+-------------------------------------------------+" <<endl;
    cout << "Order ID      : " << order.orderId << endl;
    cout << "Customer Name : " << order.customerName << endl;

    Table detailTable;
    detailTable.add_row({"Item Name", "Quantity", "Price ($)", "Subtotal ($)"});
    detailTable[0].format().font_style({FontStyle::bold});

    double calculatedTotal = 0.0;
    for (const auto& item : order.products) {
        const auto& product = item.first;
        int quantity = item.second;
        double price = product.sellPrice;
        if (quantity < 0 || price < 0) {
            throw runtime_error("Invalid item data in order " + order.orderId);
        }
        ostringstream priceStream, subtotalStream;
        priceStream << fixed << setprecision(2) << price;
        double subtotal = quantity * price;
        subtotalStream << fixed << setprecision(2) << subtotal;
        detailTable.add_row({
            product.name,
            to_string(quantity),
            priceStream.str(),
            subtotalStream.str()
        });
        calculatedTotal += subtotal;
    }

    cout << detailTable << endl;
    cout << "Date          : " << (order.dateTime.empty() ? "N/A" : order.dateTime) << endl;
    cout << "Total         : $" << fixed << setprecision(2) << calculatedTotal << endl;
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    system("cls");
}

void ProductManagementSystem::displayOrderTable(const vector<Order>& orders, int currentPage, const string& title) {
    const int pageSize = 5;
    int totalPages = (orders.size() + pageSize - 1) / pageSize;

    system("cls");
    cout << "+-----------------------------------------------------------------------+" <<endl;
    cout << "|                      " << title << " (Page " << currentPage << " of " << totalPages << ")                         |\n";
    cout << "+-----------------------------------------------------------------------+" <<endl;

    Table table;
    table.add_row({"No", "Order ID", "Customer Name", "Total ($)", "Date"});
    table[0].format().font_style({FontStyle::bold});

    size_t start = (currentPage - 1) * pageSize;
    size_t end = min(start + pageSize, orders.size());

    for (size_t i = start; i < end; ++i) {
        ostringstream totalStream;
        totalStream << fixed << setprecision(2) << orders[i].total;
        table.add_row({
            to_string(i + 1),
            orders[i].orderId,
            orders[i].customerName,
            totalStream.str(),
            orders[i].dateTime.empty() ? "N/A" : orders[i].dateTime
        });
    }

    table.format().font_align(FontAlign::center).padding_left(1).padding_right(1);
    cout << table << endl;
    cout << "📕 Page [" << currentPage << " of " << totalPages << "]\n";
    cout << "[➡️ ] Next | [⬅️ ] Previous | [ESC] Back\n";
}

void ProductManagementSystem::viewStaffOrders() {
    if (orders.empty()) {
        cout << "\n  ‼️ No orders have been placed yet.\n";
        presskeyEnter();
        return;
    }

    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    stringstream dateStream;
    dateStream << put_time(&localTime, "%d %b %Y");
    string today = dateStream.str(); 

    vector<Order> sortedOrders = orders;
    sort(sortedOrders.begin(), sortedOrders.end(), [](const Order& a, const Order& b) {
        return a.total > b.total;
    });

    const vector<string> menuOptions = {
        "1️⃣  Order Inventory Summary",
        "2️⃣  View All Orders",
        "3️⃣  View Orders by Day",
        "4️⃣  View Order Details by No or Order ID",
        "5️⃣  Delete Order by UUID"
    };
    int selectedOption = 0;

    while (true) {
        system("cls");
        cout << "\n+============================================+\n";
        cout << "|              STAFF ORDER MENU              |\n";
        cout << "+============================================+\n";
        for (size_t i = 0; i < menuOptions.size(); ++i) {
            cout << (i == selectedOption ? "👉 " : "   ") << " " << menuOptions[i] << "\n";
        }
        cout << "==============================================\n";
        cout << "↕️  Use arrow key to navigate and [ESC] Return\n";

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int arrow = _getch();
            if (arrow == 72) { 
                selectedOption = (selectedOption == 0) ? menuOptions.size() - 1 : selectedOption - 1;
            } else if (arrow == 80) { 
                selectedOption = (selectedOption + 1) % menuOptions.size();
            }
        } else if (ch == 13) { 
            switch (selectedOption) {
        case 0: { 
            system("cls");
            cout << "+=================================================+" << endl;
            cout << "|             ORDER INVENTORY SUMMARY             |" << endl;
            cout << "+=================================================+" << endl;
            cout << " Date: " << today << "\n";

            map<string, pair<int, double>> inventory; // name -> (qty, revenue)
            map<string, double> profitMap;            // name -> profit
            map<string, double> marginMap;            // name -> margin %

            double finalTotal = 0.0, totalProfit = 0.0;
            int totalQty = 0;

            bool hasValidItems = false;

            for (const auto& order : orders) {
                if (!order.dateTime.empty() && order.dateTime.find(today) != string::npos) {
                    for (const auto& item : order.products) {
                        const auto& product = item.first;
                        int quantity = item.second;
                        if (quantity <= 0) continue;

                        double sellPrice = product.sellPrice;
                        double costPrice = product.costPrice;

                        if (costPrice < 0.0 || costPrice > sellPrice * 10) {
                            cerr << "‼️ Invalid cost price for: " << product.name << ". Defaulting to 50% of sell price.\n";
                            costPrice = 0.5 * sellPrice;
                        }

                        double revenue = quantity * sellPrice;
                        double profit = quantity * (sellPrice - costPrice);
                        double margin = (revenue > 0.0) ? (profit / revenue) * 100.0 : 0.0;

                        if (margin > 999.9 || margin < -999.9) margin = 999.9;

                        inventory[product.name].first += quantity;
                        inventory[product.name].second += revenue;
                        profitMap[product.name] += profit;
                        marginMap[product.name] = margin;

                        totalQty += quantity;
                        finalTotal += revenue;
                        totalProfit += profit;
                        hasValidItems = true;
                    }
                }
            }

            if (!hasValidItems) {
                cout << "\n  ‼️ No valid orders found for " << today << ".\n";
            } else {
                Table summaryTable;
                summaryTable.add_row({"Item Name", "Quantity", "Total ($)", "Profit ($)", "Margin (%)"});
                summaryTable[0].format().font_style({FontStyle::bold});

                string topItem;
                double topRevenue = 0.0;

                for (const auto& entry : inventory) {
                    const string& name = entry.first;
                    int qty = entry.second.first;
                    double revenue = entry.second.second;
                    double profit = profitMap[name];
                    double margin = marginMap[name];

                    if (revenue > topRevenue) {
                        topRevenue = revenue;
                        topItem = name;
                    }

                    ostringstream totalStream, profitStream, marginStream;
                    totalStream << fixed << setprecision(2) << revenue;
                    profitStream << fixed << setprecision(2) << profit;
                    marginStream << fixed << setprecision(1) << margin << "%";

                    summaryTable.add_row({
                        name,
                        to_string(qty),
                        totalStream.str(),
                        profitStream.str(),
                        marginStream.str()
                    });
                }

                summaryTable.format().font_align(FontAlign::center).padding_left(1).padding_right(1);
                cout << summaryTable << endl;

                cout << left << setw(14) << " - Total Items" << ": " << inventory.size() << endl;
                cout << setw(14) << " - Total Qty" << ": " << totalQty << endl;
                cout << setw(14) << " - Final Total" << ": $" << fixed << setprecision(2) << finalTotal << endl;
                cout << setw(14) << " - Total Profit" << ": $" << fixed << setprecision(2) << totalProfit << endl;
                cout << setw(14) << " - Top Item" << ": " << topItem << " ($" << fixed << setprecision(2) << topRevenue << ")" << endl;
            }
            presskeyEnter();
            break;
        }

                case 1: { 
                    int currentPage = 1;
                    while (true) {
                        displayOrderTable(sortedOrders, currentPage, "ALL ORDERS");
                        int key = _getch();
                        if (key == 27) break; 
                        if (key == 0 || key == 224) {
                            int arrow = _getch();
                            int totalPages = (sortedOrders.size() + 4) / 5;
                            if (arrow == 75 && currentPage > 1) currentPage--; 
                            else if (arrow == 77 && currentPage < totalPages) currentPage++; 
                        }
                    }
                    break;
                }
                case 2: { 
                    system("cls");
                    cout << "+=================================================+" <<endl;
                    cout << "|                VIEW ORDERS BY DAY               |\n";
                    cout << "+=================================================+" <<endl;
                    cout << " - Enter date (DD MM YYYY): ";
                    string dateInput;
                    getline(cin, dateInput);

                    int day, month, year;
                    stringstream ss(dateInput);
                    if (ss >> day >> month >> year && day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 2000) {
                        stringstream dateStream;
                        std::tm tm_date = {};
                        tm_date.tm_mday = 1;
                        tm_date.tm_mon = month - 1;
                        tm_date.tm_year = year - 1900;
                        dateStream << setfill('0') << setw(2) << day << " "
                                   << std::put_time(&tm_date, "%b") << " "
                                   << year;
                        string targetDate = dateStream.str();

                        vector<Order> dayOrders;
                        for (const auto& order : sortedOrders) {
                            if (!order.dateTime.empty() && order.dateTime.substr(4, 11) == targetDate) {
                                dayOrders.push_back(order);
                            }
                        }

                        if (dayOrders.empty()) {
                            cout << "\n  ‼️ No orders found for " << targetDate << ".\n";
                            cout << "Press Enter to continue...";
                            presskeyEnter();
                            break;
                        }

                        int currentPage = 1;
                        while (true) {
                            displayOrderTable(dayOrders, currentPage, "ORDERS FOR " + targetDate);
                            int key = _getch();
                            if (key == 27) break; 
                            if (key == 0 || key == 224) {
                                int arrow = _getch();
                                int totalPages = (dayOrders.size() + 4) / 5;
                                if (arrow == 75 && currentPage > 1) currentPage--;
                                else if (arrow == 77 && currentPage < totalPages) currentPage++; 
                            }
                        }
                    } else {
                        cout << "\n  ‼️ Invalid date format! Use DD MM YYYY (12 06 2025).\n";
                        cout << "Press Enter to continue...";
                        presskeyEnter();
                    }
                    break;
                }
                case 3: { 
                    system("cls");
                    cout << "+=================================================+" <<endl;
                    cout << "|               VIEW ORDER DETAILS                |\n";
                    cout << "+=================================================+" <<endl;
                    cout << " - Enter order number or Order ID: ";
                    string input;
                    getline(cin, input);

                    if (input.empty()) {
                        cout << "\n  ‼️ Input cannot be empty.\n";
                        cout << "Press Enter to continue...";
                        presskeyEnter();
                        break;
                    }

                    try {
                        size_t index = stoul(input) - 1;
                        if (index < sortedOrders.size()) {
                            displayOrderDetails(sortedOrders[index]);
                        } else {
                            cout << "\n  ‼️ Order number out of range (1-" << sortedOrders.size() << ").\n";
                            cout << "Press Enter to continue...";
                            presskeyEnter();
                        }
                    } catch (const invalid_argument&) {
                        auto it = find_if(sortedOrders.begin(), sortedOrders.end(),
                            [&input](const Order& o) { return o.orderId == input; });
                        if (it != sortedOrders.end()) {
                            displayOrderDetails(*it);
                        } else {
                            cout << "\n  ‼️ Order ID '" << input << "' not found.\n";
                            cout << "Press Enter to continue...";
                            presskeyEnter();
                        }
                    }
                    break;
                }
                case 4: { 
                    system("cls");
                    cout << "+=================================================+" << endl;
                    cout << "|               DELETE ORDER BY UUID              |" << endl;
                    cout << "+=================================================+" << endl;

                    while (true) {
                        string uuid = inputWithCancel(" - Enter Order UUID to delete: ");
                        if (uuid.empty()) {
                            cout << "\n 🔁 Deletion canceled.\n";
                            //presskeyEnter();
                            break;
                        }

                        auto it = remove_if(orders.begin(), orders.end(), [&uuid](const Order& o) {
                            return o.orderId == uuid;
                        });

                        if (it != orders.end()) {
                            orders.erase(it, orders.end());
                            saveOrders();
                            cout << " ✅ Order with UUID " << uuid << " deleted successfully!\n";
                        } else {
                            cout << " ❌ No order found with UUID: " << uuid << "\n";
                        }

                        string cont;
                        cout << " - Delete another order? (y/n): ";
                        getline(cin, cont);
                        if (cont != "y" && cont != "Y") {
                            break;
                        }

                        system("cls");
                        cout << "+=================================================+" << endl;
                        cout << "|               DELETE ORDER BY UUID              |" << endl;
                        cout << "+=================================================+" << endl;
                    }
                    break;
                }
            }
        } else if (ch == 27) { 
            break;
        }
    }
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
        cout << "+=================================================+" <<endl;
        cout << "|                 REPORT DASHBOARD                |\n";
        cout << "+=================================================+" <<endl;

        for (int i = 0; i < optionCount; ++i) {
            if (i == selected){
                cout << " 👉 " << options[i] << "\n";  
            }else{
                cout << "    " << options[i] << "\n";
            }
        }

        cout << "+=================================================+" <<endl;
        cout << "↕️  Use arrow keys to navigate.";

        int key = _getch();
        if (key == 224) { 
            key = _getch(); 
            if (key == 72 && selected > 0) selected--;        
            else if (key == 80 && selected < optionCount - 1) selected++; 
        } else if (key == 13) { 
            switch (selected) {

                case 0: {
                    system("cls");
                    generateInventorySummary();
                    break;
                }
                case 1: 
                viewLowStockProducts(); 
                break;
                case 2: {
                    cout << endl;
                    viewHighStockProducts();
                    presskeyEnter();
                    break;
                }
                case 3: exportProductReport(); 
                break;
                case 4: 
                exportDataToCsv(); 
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
                cout << " ✅ Report exported to products_export.csv\n";
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
        cout << "+=================================================+" <<endl;
        cout << "|               EXPORT PRODUCT REPORT             |\n";
        cout << "+=================================================+" <<endl;
        for (int i = 0; i < MENU_SIZE; ++i) {
            if (i == selected)
                cout << " 👉 " << options[i] << endl;
            else
                cout << "    " << options[i] << endl;
        }
        cout << "+=================================================+" <<endl;
        cout << "↕️  Use arrows to navigate, ENTER to select.";

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

    cout << "✅  Data exported successfully to:\n";
    cout << "- products_report.csv\n";
    cout << "- categories_report.csv\n";
    cout << "- users_report.csv\n";
    cout << "- orders_report.csv\n";
    cout << "- restock_report.csv\n";
}

void ProductManagementSystem::generateInventorySummary() {
    double totalBaseValue = 0, totalSellValue = 0;
    int totalQuantity = 0, lowStock = 0, unavailable = 0;
    int highStockThreshold = 100;
    vector<string> highStockItems;

    const Product* mostStocked = nullptr;
    const Product* highestPriced = nullptr;

    time_t now = time(0);
    tm* localTime = localtime(&now);
    char dateStr[100];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

    for (const auto& product : products) {
        totalBaseValue += product.basePrice * product.quantity;
        totalSellValue += product.totalPrice;
        totalQuantity += product.quantity;

        if (product.quantity < 10) lowStock++;
        if (product.quantity == 0) unavailable++;
        if (product.quantity >= highStockThreshold)
            highStockItems.push_back(product.name);

        if (!mostStocked || product.quantity > mostStocked->quantity)
            mostStocked = &product;

        if (!highestPriced || product.sellPrice > highestPriced->sellPrice)
            highestPriced = &product;
    }

    cout << "+====================================================+" <<endl;
    cout << "|                 INVENTORY SUMMARY                  |\n";
    cout << "+====================================================+" <<endl;
    cout << "- Date.............................: " << dateStr << endl;
    cout << "- Total Categories.................: " << categories.size() << endl;
    cout << "- Total Products...................: " << products.size() << endl;
    cout << "- Total Stock Quantity.............: " << totalQuantity << endl;
    cout << "- Total Base Value.................: $" << fixed << setprecision(2) << totalBaseValue << endl;
    cout << "- Total Sell Value.................: $" << totalSellValue << endl;
    cout << "- Low Stock Items (<10)............: " << lowStock << endl;
    cout << "- Unavailable Items (0 qty)........: " << unavailable << endl;
    cout << "- Most Stocked Product.............: " << (mostStocked ? mostStocked->name : "None") << endl;
    cout << "- Highest Priced Product...........: " << (highestPriced ? highestPriced->name : "None") << endl;
    cout << "- High Stock Items (≥" << highStockThreshold << ").....: "
         << (highStockItems.empty() ? "None" : "") << endl;
    for (const auto& item : highStockItems)
        cout << "  • " << item << endl;
    cout << "+====================================================+" <<endl;

    cout << "▶️  Press [S] to save inventory or press Enter to return: ";
    string input;
    getline(cin, input);

    if (input == "S" || input == "s") {
        ofstream outFile("inventory_summary.txt");
        if (outFile.is_open()) {
            cout << "+====================================================+" <<endl;
            cout << "|                 INVENTORY SUMMARY                  |\n";
            cout << "+====================================================+" <<endl;
            outFile << "- Date.............................: " << dateStr << "\n";
            outFile << "- Total Categories.................: " << categories.size() << "\n";
            outFile << "- Total Products...................: " << products.size() << "\n";
            outFile << "- Total Stock Quantity.............: " << totalQuantity << "\n";
            outFile << "- Total Base Value.................: $" << fixed << setprecision(2) << totalBaseValue << "\n";
            outFile << "- Total Sell Value.................: $" << totalSellValue << "\n";
            outFile << "- Low Stock Items (<10)............: " << lowStock << "\n";
            outFile << "- Unavailable Items (0 qty)........: " << unavailable << "\n";
            outFile << "- Most Stocked Product.............: " << (mostStocked ? mostStocked->name : "None") << "\n";
            outFile << "- Highest Priced Product...........: " << (highestPriced ? highestPriced->name : "None") << "\n";
            outFile << "- High Stock Items (≥" << highStockThreshold << ").....: "
                    << (highStockItems.empty() ? "None" : "") << "\n";
            for (const auto& item : highStockItems)
                outFile << "  • " << item << "\n";
            outFile << "+====================================================+\n";
            outFile.close();
            cout << " ✅  Summary saved to 'inventory_summary.txt'\n";
        } else {
            cout << " ⚠️  Error: Could not write to file.\n";
        }
        cout << "Press Enter to return...";
        cin.get();
    } else {
        cout << "🔄️  Returning...\n";
    }
}

void ProductManagementSystem::showUnavailableProducts() {
    system("cls");   
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "|                     Unavailable Products                   |" << endl;
    cout << "+------------------------------------------------------------+" << endl;

    Table table;
    table.add_row({"No", "ID", "Name", "Size", "Category", "Qty", "Status"});
    table[0].format().font_style({FontStyle::bold});

    int count = 1;
    bool hasUnavailable = false;

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

            table.add_row({
                to_string(count++),
                product.id,
                product.name,
                displaySize,
                categoryName,
                to_string(product.quantity),
                product.status
            });
        }
    }

    if (!hasUnavailable) {
        cout << " ❌ No unavailable products!\n";
        presskeyEnter();
        return;
    }

    cout << table << endl;
    cout << "Press 'R' to restock a product, or any other key to return.\n";

    char action = _getch();
    if (action == 'R' || action == 'r') {
        while (true) {
            restockProductById();
            string continueChoice;
            cout << " - Restock another product? (y/n): ";
            getline(cin, continueChoice);
            if (continueChoice == "n" || continueChoice == "N") {
                presskeyEnter();
                break;
            } else if (continueChoice != "y" && continueChoice != "Y") {
                cout << " ‼️ Invalid input! Please enter 'y' or 'n'.\n";
            }
        }
    }
}

void ProductManagementSystem::restockProductById() {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|             Restock Product              |" << endl;
    cout << "+==========================================+" << endl;

    string id = inputWithCancel(" - Enter product ID to restock: ");
    if (id.empty()) {
        cout << " 🔁 Cancelled restock.\n";
        presskeyEnter();
        return;
    }

    for (auto& product : products) {
        if (product.id == id) {
            cout << " - Product: " << product.name << " (" << product.quantity << ")\n";

            int qty;
            while (true) {
                cout << " - Enter quantity to add: ";
                string input;
                getline(cin, input);
                try {
                    qty = stoi(input);
                    if (qty <= 0) {
                        cout << " ‼️ Invalid quantity! Must be positive.\n";
                        continue;
                    }
                    break;
                } catch (...) {
                    cout << " ‼️ Invalid input! Please enter a valid integer.\n";
                }
            }

            product.quantity += qty;
            product.totalPrice = product.sellPrice * product.quantity;
            product.status = (product.quantity > 0) ? "Available" : "Unavailable";

            restockLogs.emplace_back(product.id, product.name, qty, getCurrentDate());
            saveProducts();
            saveRestockLogs();

            cout << " ✅ Product " << product.name << " restocked successfully! New Qty: " << product.quantity << endl;
            return;
        }
    }

    cout << " ❌ Product ID not found!\n";
}

void ProductManagementSystem::showRestockLogs(int pageSize) {
    system("cls");
    cout << "\n+==========================================+" << endl;
    cout << "|             Restock Log                 |" << endl;
    cout << "+==========================================+" << endl;

    if (restockLogs.empty()) {
        cout << " ❌ No restock logs!\n";
        presskeyEnter();
        return;
    }

    int totalLogs = restockLogs.size();
    int totalPages = (totalLogs + pageSize - 1) / pageSize;
    int currentPage = 1;

    while (true) {
        system("cls");
        cout << "\n+==========================================+" << endl;
        cout << "|             Restock Log                 |" << endl;
        cout << "+==========================================+" << endl;

        Table table;
        table.add_row({"ProductID", "ProductName", "QtyRestocked", "RestockDate"});
        table[0].format().font_style({FontStyle::bold});

        int startIdx = (currentPage - 1) * pageSize;
        int endIdx = std::min(startIdx + pageSize, totalLogs);

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& log = restockLogs[i];
            table.add_row({
                log.productId,
                log.productName,
                to_string(log.quantityRestocked),
                log.restockDate
            });
        }

        cout << table << endl;
        cout << "📕  Page [" << currentPage << " of " << totalPages << "]\n";
        cout << "[➡️  ] Next | [⬅️  ] Previous | [ESC] Quit Page\n";

        int ch = _getch();

        if (ch == 27) { 
            break;
        } else if (ch == 0 || ch == 224) {
            int arrow = _getch();
            if (arrow == 75 && currentPage > 1) { 
                currentPage--;
            } else if (arrow == 77 && currentPage < totalPages) { 
                currentPage++;
            }
        }
    }
}

