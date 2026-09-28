# Library Management System (C++)

A comprehensive, console-based **Library Management System** built in C++ using **Object-Oriented Programming (OOP)**, **Persistent Binary File Handling**, and **Menu-Driven Workflows**.

---

## 🏛️ Criteria & Key Features Alignment

| Requirement | Implementation Details |
| :--- | :--- |
| **Classes / Structures for Books & Members** | • `class Book`: Encapsulates Book ID, Title, Author, Genre, Total Stock, and Available Copies.<br>• `class Member`: Encapsulates Member ID, Name, Phone, and Borrowed Book Array.<br>• `class IssueRecord`: Encapsulates Transaction ID, timestamps, return status, and fines. |
| **Book Issue & Return** | • **Issue Workflow**: Enforces member validity, 3-book borrow limit, and stock availability.<br>• **Return Workflow**: Automatically calculates overdue fines, replenishes stock, and marks loan resolved. |
| **Search by Title, Author, ID, or Genre** | • Multi-criteria search engine with case-insensitive matching for book titles, authors, IDs, and genre filters. |
| **Persistent File Storage** | • Uses `fstream` (`ios::binary`) to persistently store records in `books.dat`, `members.dat`, and `issues.dat`. |

---

## 🌟 Unique Features & Enhancements

1. **Automated Overdue Fine & Late Fee Calculator**:
   - Implements a 14-day lending cycle.
   - Calculates days overdue and assesses a configurable daily late fee (Rs. 5.00/day) upon return.

2. **Digital Issue & Return Slips**:
   - Generates formatted ASCII transaction receipts with date stamps, due date, member details, and fine summaries for both checkouts and returns.

3. **Member Borrowing Limit (Max 3 Books)**:
   - Restricts active loans to 3 books per member to ensure equitable library circulation.
   - Dedicated member profile view showing active loans and borrowed book IDs.

4. **Genre / Category Filtering**:
   - Books can be filtered by genre (e.g. Computer Science, Science, Fiction, History, Business, etc.).

5. **Library Statistics & Inventory Dashboard**:
   - Displays real-time metrics:
     - Total Titles & Total Physical Copies
     - Copies Available on Shelves vs. Copies on Loan
     - Registered Members & Active Borrowers
     - Total Completed Loans & Active Loans
     - Total Overdue Fines Collected (Rs.)

---

## 🛠️ How to Compile and Run

### Step 1: Open Terminal / PowerShell
```powershell
cd "c:\Users\kirti\OneDrive\Desktop\Thiranex Project\Project4_LibraryManagement"
```

### Step 2: Compile with `g++`
```powershell
g++ -O2 -Wall library_management.cpp -o library_management.exe
```

### Step 3: Run the Application
```powershell
.\library_management.exe
```

---

## 📋 Menu Navigation Guide

| Option | Operation | Key Action |
| :---: | :--- | :--- |
| **[1]** | Add New Book to Inventory | Sets ID, Title, Author, Genre, and Total Copies |
| **[2]** | View Entire Book Catalog | Master tabular stock view |
| **[3]** | Search Books | By Title, Author, ID, or Genre filter |
| **[4]** | Issue Book (Checkout) | Checks member limit, decrements stock, prints slip |
| **[5]** | Return Book | Calculates late fine, replenishes stock, prints return receipt |
| **[6]** | Register New Member | Enrolls member with contact info |
| **[7]** | View All Members | Shows member directory and active loan counts |
| **[8]** | View Member Profile | Inspects active borrowed book IDs |
| **[9]** | Library Statistics Dashboard | Executive overview of stock, circulation, and fines |
| **[10]**| Exit System | Clean termination |
