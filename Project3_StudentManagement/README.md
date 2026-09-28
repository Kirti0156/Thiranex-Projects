# Student Management System (C++)

A console-based **Student Management System** developed in C++ that efficiently manages student records using **Object-Oriented Programming (OOP)**, **Persistent Binary File Handling**, and **Menu-Driven Operations**.

---

## 🎯 Criteria Alignment

| Requirement | Implementation Details |
| :--- | :--- |
| **Add, Update, Delete & Display** | • Complete CRUD functionality with uniqueness checks, interactive updates, safe file reorganization on deletion, and master tabular views. |
| **Persistent File Handling** | • Uses `fstream` with `ios::binary` to store records in `student_records.dat`. Records persist across program restarts and are protected from plaintext tampering. |
| **Menu-Driven Operations** | • Clean, numbered menu with robust input validation preventing invalid inputs or infinite loops. |

---

## 🌟 Unique Features & Enhancements

1. **Automated Multi-Subject Grading & GPA Engine**:
   - Stores marks across 5 core subjects:
     1. Mathematics
     2. Physics
     3. Chemistry
     4. Computer Science
     5. English
   - Automatically computes Total Marks (out of 500), Overall Percentage, Letter Grade (`A+`, `A`, `B`, `C`, `D`, `F`), and GPA on a 10.0 scale.
   - Enforces a minimum pass mark (40/100) per subject.

2. **ASCII Academic Report Card / Marksheet**:
   - Generates a styled student marksheet displaying subject-wise scores, maximum marks, individual pass/fail status, total score, percentage, GPA, and final promotion status.

3. **Multi-Criteria Search Engine**:
   - **By Roll Number**: Fast exact lookup to view the student's full report card.
   - **By Student Name**: Case-insensitive substring matching (e.g. searching "john" finds "John Doe").
   - **By Grade Filter**: Instantly display all students holding a specific grade (e.g., all `A+` or `A` students).

4. **Classroom Analytics & Statistics Dashboard**:
   - Calculates and visualizes key metrics across all enrolled students:
     - Total Enrolled Students
     - Class Average Percentage
     - Class Topper (Highest score, name, and roll number)
     - Lowest Scorer
     - Pass / Fail Counts and Pass Rate (%)

---

## 🛠️ How to Compile and Run

### Step 1: Open Terminal / PowerShell
```powershell
cd "c:\Users\kirti\OneDrive\Desktop\Thiranex Project\Project3_StudentManagement"
```

### Step 2: Compile with `g++`
```powershell
g++ -O2 -Wall student_management.cpp -o student_management.exe
```

### Step 3: Run the Program
```powershell
.\student_management.exe
```

---

## 📋 Menu Navigation Guide

| Option | Operation | Description |
| :---: | :--- | :--- |
| **[1]** | Add New Student Record | Input roll number, name, course, and 5 subject marks |
| **[2]** | Display All Students | Formatted master directory table |
| **[3]** | Search Student | Search by Roll Number, Name, or Grade filter |
| **[4]** | Generate Academic Report Card | Printable marksheet transcript for a specific student |
| **[5]** | Update Student Record | Edit name, course, or re-enter subject marks in-place |
| **[6]** | Delete Student Record | Permanently removes student with confirmation |
| **[7]** | Classroom Analytics Dashboard | View class average, topper, and pass percentage |
| **[8]** | Exit Application | Safely exits the system |
