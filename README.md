# 🏦 Bank Management System (C++)

A robust, console-based Bank Management System built using C++. This project demonstrates foundational programming principles, secure user session management, file handling, and an advanced administrative permissions system utilizing Bitwise operators.

---

## ✨ Features

### 👥 Main Menu Options
* 📜 **Show Client List:** Displays all banking clients in a formatted grid (Account Number, Pin Code, Name, Phone, and Balance).
* ➕ **Add New Client:** Registers new clients with real-time file searching to prevent duplicate account numbers.
* ❌ **Delete Client:** Performs soft-deletion and cleans up the database by filtering out marked clients.
* 📝 **Update Client Info:** Allows updating an existing client's Pin Code, Name, Phone, and Balance.
* 🔍 **Find Client:** Instantly searches for and displays comprehensive details of a specific client.
* 💰 **Transactions Menu:** Accesses the sub-menu for managing financial deposits, withdrawals, and tracking absolute total balances.
* 🛠️ **Manage Users Menu:** Administrative portal reserved for managing system operators, credentials, and custom permissions.

### 🔐 Users & Session Security System
* 🔑 **Controlled Login Screen (`LoginScreen`):** Features a resilient authentication loop that restricts access until valid user credentials matching the records inside `Users.txt` are verified.
* 🔄 **Session Hard-Reset Mechanism (`ResetCurrentUser`):** Enforces a strict memory-clearing security protocol. The current user data is fully purged from the system context under two safe-guards:
  * Immediately upon a failed authentication attempt to prevent credential leaking.
  * Right after choosing the **Logout [8]** option from the Main Menu before routing back to the Login Screen.
* 🛡️ **Bitwise Permissions Guard:** Evaluates operator entry rights against specific modules (Clients, Transactions, Users) via dynamic bitmask matching. Supports explicit complete full-access codes (`-1`).

---

## 🛠️ Tech Stack & Architecture

* **Language:** C++ (`std::vector`, File Stream handling, Object Manipulation)
* **Storage Layer:** Flat-file Database Architecture (`Clients.txt` and `Users.txt`) using customized token delimiters (`#//#`).
* **Design Pattern:** Modular Structural Programming, utilizing strict structures (`stClientData`, `stUser`) and comprehensive operational enumerations (`enMainMenuOptions`, `enTransactions`, `enManageUserOptions`).

---

## 🚀 Execution Flow

The software structure operates seamlessly across the following core workflow:
`main()` ➡️ `LoginScreen()` ➡️ `ShowMainMenu()` ➡️ `PerformMainMenuOption()` ➡️ *(Respective Screens / Logout Reset)*
