# 🚀 Thiranex C++ Project Portfolio

A comprehensive collection of 4 console-based C++ applications developed as part of the **Thiranex Internship**. These projects demonstrate core programming paradigms, **Object-Oriented Programming (OOP)**, **Persistent Binary File Handling**, **Input Validation**, and **Real-World Application Logic**.

---

## 📂 Projects Overview

| # | Project Name | Description | Key Topics / Highlights | Folder |
| :-: | :--- | :--- | :--- | :-: |
| **01** | **Snake Game** | Arcade-style classic snake game with dynamic rendering and score tracking. | Game loops, 1D Coordinate Arrays, Collision Detection, Flicker-free console rendering | [`Project1_Snake/`](./Project1_Snake) |
| **02** | **Bank Management System** | Secure financial account management simulation with masked PIN protection. | OOP Encapsulation, 4-Digit Masked PIN, Binary Stream I/O (`.dat`), In-place Record Updates, Fund Transfers | [`Project2_BankManagement/`](./Project2_BankManagement) |
| **03** | **Student Management System** | Academic student records manager with automated grade and GPA calculation. | Full CRUD Operations, 5-Subject Academic Scoring, Automatic GPA Engine, ASCII Marksheet / Report Cards | [`Project3_StudentManagement/`](./Project3_StudentManagement) |
| **04** | **Library Management System** | Comprehensive library catalog and book loan circulation system. | Multiple Classes (`Book`, `Member`, `IssueRecord`), Overdue Fine Calculation, Stock Tracking, Checkout Slips | [`Project4_LibraryManagement/`](./Project4_LibraryManagement) |

---

## 🛠️ Tech Stack & Concepts

- **Language:** C++ (C++11 or higher)
- **Compiler:** GCC / MinGW (`g++`), Clang, or MSVC
- **Architecture & Design:**
  - **Object-Oriented Design (OOD):** Encapsulation, Data Hiding, Constructor initialization, and Modular Member Functions.
  - **Data Persistence:** Robust file stream processing (`fstream`, `ios::binary`, `seekp`/`seekg`) ensuring data integrity across sessions.
  - **Input Sanitization:** Keystroke masking, buffer flushing, and state-validation preventing runtime exceptions and infinite loops.
  - **Terminal User Interface (TUI):** Clean ASCII menus, formatted reporting tables, and cursor manipulation (`windows.h`).

---

## 📁 Repository Directory Structure

```text
Thiranex-Projects/
├── Project1_Snake/
│   ├── snake_game.cpp          # Game loop, array tail tracking, rendering
│   └── README.md               # Controls, rules, and compilation guide
├── Project2_BankManagement/
│   ├── bank_management.cpp     # Banking operations, PIN security, file persistence
│   └── README.md               # Security features, class structure, and usage
├── Project3_StudentManagement/
│   ├── student_management.cpp  # Student CRUD, GPA calculation, report cards
│   └── README.md               # Marksheet structure, grading scale, instructions
├── Project4_LibraryManagement/
│   ├── library_management.cpp  # Book catalog, member loans, overdue fine calculator
│   └── README.md               # Workflow guide, class design, navigation menu
├── .gitignore                  # Excludes compiled .exe binaries and cache
└── README.md                   # Repository overview & navigation (this file)
```

---

## ⚡ How to Compile and Run

### Prerequisites
Make sure you have a C++ compiler installed (e.g., MinGW GCC or MSVC). Verify by running:
```bash
g++ --version
```

### Running Any Project
Navigate into the respective project folder, compile the `.cpp` file, and execute:

#### 1. Snake Game
```powershell
cd Project1_Snake
g++ -o snake_game.exe snake_game.cpp
.\snake_game.exe
```

#### 2. Bank Management System
```powershell
cd Project2_BankManagement
g++ -o bank_management.exe bank_management.cpp
.\bank_management.exe
```

#### 3. Student Management System
```powershell
cd Project3_StudentManagement
g++ -o student_management.exe student_management.cpp
.\student_management.exe
```

#### 4. Library Management System
```powershell
cd Project4_LibraryManagement
g++ -o library_management.exe library_management.cpp
.\library_management.exe
```

---

## 👤 Author
- **Kirti Patil** - [GitHub Profile](https://github.com/Kirti0156)
