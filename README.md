# 💰 Expense Tracker System (C++)

A robust, console-based personal finance management application developed in **C++** using **Procedural Programming**. The system provides comprehensive transaction management, data persistence via file handling, full input validation, and detailed financial analytics (monthly, yearly, and all-time).

---

## 🚀 Key Features

* **Transaction Management:**
  * Add Income and Expense transactions with auto-generated unique IDs.
  * Over-draft Protection: Prevents expenses from exceeding the current available balance.
  * Search transactions by ID.
  * Soft-delete transactions with confirmation and file rewriting.
  * Display all transactions in a clean tabular view.

* **Advanced Financial Analytics & Statistics:**
  * **All-Time Summary:** Total income, total expenses, net balance, highest/lowest expense, and highest income/expense categories.
  * **Specific Month Analysis:** Deep dive into monthly financial habits with dynamic month-name formatting.
  * **Specific Year Analysis:** Comprehensive annual net balance and expense summaries.

* **Robust Data Validation & System Reliability:**
  * **Date Validation:** Accounts for leap years and dynamically checks day counts per month (1–28/29/30/31).
  * **Crash-Proof Input Handling:** Clears input buffers (`cin.fail()`, `cin.clear()`, `cin.ignore()`) to prevent infinite loops on non-numeric entries.
  * **Console Feedback:** Uses audio-visual alert cues for invalid inputs and green prompts on successful operations.

* **Data Persistence:**
  * Reads and saves all operations directly to `Transactions.txt` using safe custom separators (`#//#`).

---

## 🛠️ Built With

* **Language:** C++ (Standard C++11 / C++17)
* **Paradigm:** Procedural Programming & Modular Code Architecture
* **Libraries:** `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<iomanip>`, `<limits>`
* **IDE:** Microsoft Visual Studio

---

## 📋 Data Structure

The system persists data in `Transactions.txt` using structured delimiters:

```text
ID#//#Date#//#Amount#//#Category#//#Type
1#//#14/7/2026#//#5000.000000#//#Salary#//#Income
2#//#15/7/2026#//#250.000000#//#Food#//#Expense
