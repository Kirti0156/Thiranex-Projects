# Secure Bank Management System (C++)

A robust, console-based **Bank Management Application** built in C++ that simulates core banking operations. This system emphasizes **Object-Oriented Programming (OOP)**, **Persistent Binary File Handling**, and **Financial Security Best Practices**.

---

## 🏛️ Core Concepts Demonstrated

### 1. Object-Oriented Programming (OOP)
- **Encapsulation & Data Hiding**: Private attributes (`accountNumber`, `holderName`, `accountType`, `balance`, `pin`) can only be modified through controlled public member methods.
- **Constructors**: Initializes default values for safe object creation.
- **Member Functions**: Specialized operations for setup, display, deposits, withdrawals, and PIN validation.

### 2. File Management & Persistence
- **Binary Stream I/O (`fstream`, `ios::binary`)**: Customer records are written and retrieved in binary format (`bank_records.dat`), preventing casual text inspection or manual file tampering.
- **In-Place File Updating**: Modifies existing records directly in the binary file using stream positioning (`seekp`), without having to rewrite the entire database for deposits and withdrawals.
- **Atomic Deletion / File Reorganization**: Safely removes closed accounts using a temporary file swapping strategy.

---

## 🌟 Unique Features & Enhancements

1. **4-Digit Security PIN with Masked Input**:
   - Every account is secured with a 4-digit PIN.
   - When typing the PIN, keystrokes are masked with asterisks (`*`) in real-time, supporting backspace correction.
   - Cash withdrawals, balance inquiries, fund transfers, and account closures strictly require PIN authorization.

2. **Inter-Account Fund Transfer**:
   - Seamlessly transfer money between two registered accounts.
   - Verifies sender PIN, ensures minimum balance requirements, debits the sender, and credits the receiver in a single coordinated file update.

3. **Account Rules & Minimum Balance Protection**:
   - **Savings Account (`S`)**: Minimum initial deposit and balance of **Rs. 500.00**.
   - **Current Account (`C`)**: Minimum initial deposit and balance of **Rs. 1,000.00**.
   - Withdrawals that would bring an account below its minimum threshold are rejected with an informative prompt.

4. **Savings Interest Projection Calculator**:
   - Allows savings account holders to preview estimated annual returns (at a 4% annual interest rate).

5. **Timestamped Digital Transaction Receipts**:
   - Automatically outputs a clean ASCII receipt slip displaying date, time, transaction type, account number, holder name, amount, and resulting balance.

6. **Input Validation & Safety**:
   - Rejects duplicate account numbers, negative amounts, and invalid menu inputs.

---

## 🛠️ How to Compile and Run

### Step 1: Open Terminal / PowerShell
```powershell
cd "c:\Users\kirti\OneDrive\Desktop\Thiranex Project\Project2_BankManagement"
```

### Step 2: Compile
```powershell
g++ -O2 -Wall bank_management.cpp -o bank_management.exe
```

### Step 3: Run the Application
```powershell
.\bank_management.exe
```

---

## 📋 Menu Navigation Guide

| Option | Action | Authentication |
| :---: | :--- | :---: |
| **[1]** | Open New Account (Savings / Current) | Sets 4-Digit PIN |
| **[2]** | Cash Deposit | None (Open deposit) |
| **[3]** | Cash Withdrawal (Enforces min. balance) | **PIN Required** |
| **[4]** | Balance Inquiry & Interest Projection | **PIN Required** |
| **[5]** | Fund Transfer Between Accounts | **Sender PIN Required** |
| **[6]** | View Master Account Directory | Manager View |
| **[7]** | Close / Delete Account | **PIN Required** + Confirmation |
| **[8]** | Exit System | Clean exit |
