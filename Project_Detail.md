# 🛍️ Product Management System (PMS)

## 📖 Description

The **Product Management System** is a console-based C++ application designed to manage inventory, users, categories, and orders for retail stores, warehouses, or similar businesses. It supports two main user roles — **Admin** and **Staff** — each with access to specific features. The system uses a menu-driven interface with arrow-key navigation for ease of use and interaction.

---

## 👤 User Roles

### 🔐 Admin
Admins have full control over the system, including:

- 👥 **User Management:** Add, view, update, and delete users.
- 📁 **Category Management:** Create, view, update, and delete product categories.
- 📦 **Product Management:** Full CRUD operations on products, view low stock/expired items, sort products.
- 📊 **Reporting:** Generate inventory summaries and export reports to CSV.
- 🛒 **Order Monitoring:** View staff orders and search orders by day or ID.
- 🔁 **Restocking:** View unavailable products, restock by product ID, and view restock logs.

### 👷 Staff
Staff users can:

- 📦 **Manage Products:** Add, view, search, update, and sort products.
- 🛒 **Place Orders:** Add products to cart, view/update cart, and confirm orders.
- 📑 **View Orders:** Check orders placed previously.

---

## 📂 Features by Menu

### 🔑 Login Menu
- 1️⃣ Admin Login  
- 2️⃣ Staff Login  
- 🅾️ Exit

### 🧠 Admin Dashboard
- 1️⃣ Manage Users  
- 2️⃣ Manage Categories  
- 3️⃣ Manage Products  
- 4️⃣ Generate Report  
- 5️⃣ View Staff Orders  
- 6️⃣ Restock Products  
- 🅾️ Logout

### 👥 User Management
- 1️⃣ Add New User  
- 2️⃣ View All Users  
- 3️⃣ Update Role/Password  
- 4️⃣ Delete User  
- 🅾️ Back

### 🗂️ Category Management
- 1️⃣ Add New Category  
- 2️⃣ View All Categories  
- 3️⃣ Update Category  
- 4️⃣ Delete Category  
- 🅾️ Back

### 📦 Product Management
(Admin and Staff menus differ slightly)

Admin:
- 1️⃣ Add New Product  
- 2️⃣ View All Products  
- 3️⃣ Search Product  
- 4️⃣ Update Product  
- 5️⃣ Delete Product  
- 6️⃣ View Low Stock  
- 7️⃣ Sort Products  
- 8️⃣ View Expired Products  
- 🅾️ Back

Staff:
- 1️⃣ Add New Product  
- 2️⃣ View All Products  
- 3️⃣ Search Product  
- 4️⃣ Update Product  
- 5️⃣ View Low Stock  
- 6️⃣ Sort Products  
- 🅾️ Back

### 📊 Report Dashboard
- 1️⃣ Inventory Summary  
- 2️⃣ View Low Stock  
- 3️⃣ View High Stock  
- 4️⃣ Export Product Report  
- 5️⃣ Export All Data to CSV  
- 🅾️ Back

### 📑 Staff Order Menu
- 1️⃣ Order Inventory Summary  
- 2️⃣ View All Orders  
- 3️⃣ View Orders by Day  
- 4️⃣ View Order Details (by ID or Order Number)

### 🔁 Restock Products
- 1️⃣ View Unavailable Products (Qty = 0)  
- 2️⃣ Restock by Product ID  
- 3️⃣ View Restock Log  
- 🅾️ Back

### 🛒 Place Order (Staff)
- 1️⃣ Add Product to Cart  
- 2️⃣ View Cart  
- 3️⃣ Update Cart  
- 4️⃣ Confirm Order  
- 🅾️ Back

---

## 💾 Data Management

- File-based storage using `.txt` files:
  - `users.txt`
  - `categories.txt`
  - `products.txt`
  - `orders.txt`
  - `restock_log.txt`
- All data is read from and written to files using standard C++ file streams.

---

## 🛠️ Technologies Used

- **Language:** C++
- **UI:** Console-based (Arrow Key Navigation)
- **Data Structures:** `std::vector`, `std::string`
- **Storage:** File I/O (`ifstream`, `ofstream`)

---

## 📦 Use Cases

- Retail inventory systems
- Warehouse management
- Staff-assisted product ordering systems
- Educational projects to demonstrate object-oriented programming and file handling

---

## ✅ Future Improvements

- Switch to database (e.g., SQLite) for better scalability
- GUI implementation using frameworks like Qt or wxWidgets
- Role-based login authentication using hashed passwords
- Export reports in Excel format

---

## 👨‍💻 Author

- Developed by [Your Name]
- Foundation Year Student, ISTAD

---