# 💰 Expense Tracker System (C++)

A robust, console-based personal finance and expense tracking system developed in **C++** using **procedural programming** and clean architectural modularity. The application provides end-to-end management for personal income and expenses, file-based data persistence, input validation, and statistical financial analytics.

---

## 📌 Table of Contents
- [Key Features](#-key-features)
- [System Architecture & Concepts](#-system-architecture--concepts)
- [Screenshots & Menus](#-screenshots--menus)
- [Data Storage Format](#-data-storage-format)
- [How to Build & Run](#-how-to-build--run)
- [Author](#-author)

---

## 🚀 Key Features

### 1. 💵 Financial Operations
* **Add Income:** Record income transactions with custom categories, amounts, and dates. Automatically assigns unique sequential transaction IDs.
* **Add Expense (Overdraft Protection):** Prevents logging expenses exceeding the current available balance, protecting the account from negative balances.
* **View Balance:** Real-time summary showing total income, total expenses, and net balance.
* **Search & Delete:** Quick search by ID and safe deletion with record re-indexing and file rewriting.

### 2. 📅 Intelligent Calendar & Date Engine
* **Leap Year Logic:** Accurately calculates February days (28 vs. 29 days) based on standard leap year conditions (`Year % 400 == 0 || (Year % 4 == 0 && Year % 100 != 0)`).
* **Strict Range Validation:** Rejects out-of-range days, months (1–12), or years (1900–2100).

### 3. 📊 Advanced Financial Analytics & Statistics
* **Multi-Period Filtering:** View transactions and analytics across:
  * **Specific Month:** Filter by chosen month and year.
  * **Specific Year:** Full-year aggregation.
  * **All Time:** Cumulative financial history.
* **Granular Metrics:** Computes:
  * Total transaction volume (Incomes vs. Expenses count).
  * Net savings / Net loss.
  * Highest and lowest single expense values.
  * Top spending category (Highest Expense Category).
  * Top earning stream (Highest Income Category).

### 4. 🛡️ Crash-Proof Input & Console Feedback
* **Defensive Input Handling:** Built-in safeguards (`cin.fail()`, stream clearing, and numeric limit clearing) to prevent infinite loops from invalid character input.
* **Visual & Audio Feedback:** Changes terminal background/text colors (`system("color 4F")` / `system("color 2F")`) and triggers console alert beeps (`\a`) on validation errors.

---

## 🧠 System Architecture & Concepts

The project is built around fundamental software engineering principles:
* **Procedural Programming:** Clean separation of concerns through specialized functional modules.
* **Data Persistence (File I/O):** Custom serialization and deserialization routines via `std::fstream` parsing structured delimiter strings (`#//#`).
* **In-Memory Manipulation:** `std::vector` data structures for dynamic sorting, lookup, and filtering.
* **Table Formatting:** Formatted CLI tables using `std::setw`, `std::left`, and ASCII board designs.

---

## 🖥️ Menu Overview

```text
+--------------------------------------------+
|               EXPENSE TRACKER              |
|           Personal Finance System          |
+--------------------------------------------+
|   [1]  Add Income                          |
|   [2]  Add Expense                         |
|   [3]  View Transactions                   |
|   [4]  Find Transaction                    |
|   [5]  Delete Transaction                  |
|   [6]  View Balance                        |
|   [7]  Statistics                          |
|   [8]  Exit                                |
+--------------------------------------------+
