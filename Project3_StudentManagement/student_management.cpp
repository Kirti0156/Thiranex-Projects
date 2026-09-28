/**
 * ============================================================================
 * Project: Student Management System
 * Language: C++ (Console-Based)
 * Concepts Demonstrated:
 *   1. Object-Oriented Programming (OOP)
 *      - Encapsulation & Data Hiding (private fields, public methods)
 *      - Modular Class Design & Constructors
 *   2. Persistent File Management (Binary stream I/O via fstream)
 *   3. Menu-Driven Architecture with Input Validation
 *   4. Unique Features:
 *      - Automated Multi-Subject Grading & GPA Calculation
 *      - Printable ASCII Academic Report Card / Marksheet
 *      - Multi-Criteria Search (Roll No, Name, Grade Filter)
 *      - Classroom Analytics Dashboard (Class Average, Topper, Pass Rate)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// ============================================================================
// CONSTANTS & CONFIGURATION
// ============================================================================
const char FILE_NAME[] = "student_records.dat";
const int NUM_SUBJECTS = 5;
const char SUBJECT_NAMES[NUM_SUBJECTS][20] = {
    "Mathematics",
    "Physics",
    "Chemistry",
    "Computer Science",
    "English"
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * Converts a string to uppercase for case-insensitive comparisons.
 */
string toUpperString(const string &str) {
    string upper = str;
    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    return upper;
}

// ============================================================================
// CLASS: Student (Object-Oriented Design)
// ============================================================================

class Student {
private:
    int rollNumber;
    char name[50];
    char course[30];
    double marks[NUM_SUBJECTS];
    double totalMarks;
    double percentage;
    char grade[4];
    double gpa;

    // Internal calculation method
    void calculatePerformance();

public:
    Student();

    // Data Input & Updates
    bool inputDetails(int roll);
    void updateDetails();

    // Visual Displays
    void showTabularRow() const;
    void generateReportCard() const;

    // Getters
    int getRollNumber() const { return rollNumber; }
    const char* getName() const { return name; }
    const char* getCourse() const { return course; }
    double getPercentage() const { return percentage; }
    double getTotalMarks() const { return totalMarks; }
    const char* getGrade() const { return grade; }
    double getGpa() const { return gpa; }
    bool isPassed() const;
};

// Constructor
Student::Student() {
    rollNumber = 0;
    name[0] = '\0';
    course[0] = '\0';
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        marks[i] = 0.0;
    }
    totalMarks = 0.0;
    percentage = 0.0;
    strcpy(grade, "NA");
    gpa = 0.0;
}

void Student::calculatePerformance() {
    totalMarks = 0.0;
    bool failedAnySubject = false;

    for (int i = 0; i < NUM_SUBJECTS; i++) {
        totalMarks += marks[i];
        if (marks[i] < 40.0) {
            failedAnySubject = true; // Minimum 40/100 to pass each subject
        }
    }

    percentage = totalMarks / NUM_SUBJECTS;

    // Grade and GPA evaluation
    if (failedAnySubject || percentage < 40.0) {
        strcpy(grade, "F");
        gpa = 0.0;
    } else if (percentage >= 90.0) {
        strcpy(grade, "A+");
        gpa = 10.0;
    } else if (percentage >= 80.0) {
        strcpy(grade, "A");
        gpa = 9.0;
    } else if (percentage >= 70.0) {
        strcpy(grade, "B");
        gpa = 8.0;
    } else if (percentage >= 60.0) {
        strcpy(grade, "C");
        gpa = 7.0;
    } else if (percentage >= 50.0) {
        strcpy(grade, "D");
        gpa = 6.0;
    } else {
        strcpy(grade, "E");
        gpa = 5.0;
    }
}

bool Student::isPassed() const {
    return strcmp(grade, "F") != 0;
}

bool Student::inputDetails(int roll) {
    rollNumber = roll;

    cout << "Enter Student Full Name: ";
    cin.ignore();
    cin.getline(name, 50);

    cout << "Enter Course / Department (e.g., B.Tech CSE): ";
    cin.getline(course, 30);

    cout << "\n--- Enter Marks out of 100 for 5 Core Subjects ---\n";
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        while (true) {
            cout << "  " << (i + 1) << ". " << setw(18) << left << SUBJECT_NAMES[i] << ": ";
            if (cin >> marks[i] && marks[i] >= 0.0 && marks[i] <= 100.0) {
                break;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "     >> Invalid marks! Please enter a value between 0 and 100.\n";
        }
    }

    calculatePerformance();
    return true;
}

void Student::updateDetails() {
    cout << "\nEditing Record for Roll #" << rollNumber << " (" << name << ")\n";
    cout << "Press ENTER to keep existing, or type new value:\n";

    cout << "Update Name (Current: " << name << "): ";
    cin.ignore();
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        strncpy(name, newName.c_str(), 49);
        name[49] = '\0';
    }

    cout << "Update Course (Current: " << course << "): ";
    string newCourse;
    getline(cin, newCourse);
    if (!newCourse.empty()) {
        strncpy(course, newCourse.c_str(), 29);
        course[29] = '\0';
    }

    cout << "Do you want to re-enter subject marks? (Y/N): ";
    char choice;
    cin >> choice;
    if (toupper(choice) == 'Y') {
        cout << "\n--- Enter Updated Marks (0 - 100) ---\n";
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            while (true) {
                cout << "  " << (i + 1) << ". " << setw(18) << left << SUBJECT_NAMES[i] << ": ";
                if (cin >> marks[i] && marks[i] >= 0.0 && marks[i] <= 100.0) {
                    break;
                }
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "     >> Invalid marks! Enter value between 0 and 100.\n";
            }
        }
        calculatePerformance();
    }
}

void Student::showTabularRow() const {
    cout << left << setw(10) << rollNumber
         << setw(22) << name
         << setw(16) << course
         << fixed << setprecision(1)
         << setw(10) << totalMarks
         << setw(10) << percentage
         << setw(8)  << grade
         << setw(6)  << gpa << endl;
}

void Student::generateReportCard() const {
    cout << "\n+==============================================================+";
    cout << "\n|                 ACADEMIC PERFORMANCE REPORT                  |";
    cout << "\n+==============================================================+";
    cout << "\n  Roll Number : " << setw(14) << left << rollNumber << "Course  : " << course;
    cout << "\n  Student Name: " << name;
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  SUBJECT                  MAX MARKS   OBTAINED MARKS   STATUS";
    cout << "\n+--------------------------------------------------------------+";
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        cout << "\n  " << setw(25) << left << SUBJECT_NAMES[i]
             << setw(12) << "100"
             << setw(17) << fixed << setprecision(1) << marks[i]
             << (marks[i] >= 40.0 ? "PASS" : "FAIL");
    }
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  Total Marks : " << totalMarks << " / " << (NUM_SUBJECTS * 100);
    cout << "\n  Percentage  : " << fixed << setprecision(2) << percentage << "%";
    cout << "\n  Final Grade : " << grade << "   (GPA: " << fixed << setprecision(1) << gpa << " / 10.0)";
    cout << "\n  Result      : " << (isPassed() ? "[PASSED]" : "[FAILED - MUST REAPPEAR]");
    cout << "\n+==============================================================+\n";
}

// ============================================================================
// FILE MANAGEMENT & OPERATIONS (PERSISTENCE LAYER)
// ============================================================================

bool rollExists(int roll) {
    Student temp;
    ifstream file(FILE_NAME, ios::binary);
    if (!file) return false;

    while (file.read(reinterpret_cast<char *>(&temp), sizeof(Student))) {
        if (temp.getRollNumber() == roll) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/**
 * 1. Add New Student Record
 */
void addStudentRecord() {
    cout << "\n--------------------------------------------------------";
    cout << "\n                 ADD NEW STUDENT RECORD                 ";
    cout << "\n--------------------------------------------------------\n";

    int roll;
    while (true) {
        cout << "Enter Roll Number (Unique positive integer): ";
        if (cin >> roll && roll > 0) {
            break;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << ">> Invalid input! Roll number must be a positive integer.\n";
    }

    if (rollExists(roll)) {
        cout << "\n>> [ERROR] Student with Roll Number #" << roll << " already exists!\n";
        return;
    }

    Student newStudent;
    newStudent.inputDetails(roll);

    // Save to binary file
    ofstream outFile(FILE_NAME, ios::binary | ios::app);
    if (!outFile) {
        cout << ">> [ERROR] Could not open records database file.\n";
        return;
    }
    outFile.write(reinterpret_cast<const char *>(&newStudent), sizeof(Student));
    outFile.close();

    cout << "\n>> [SUCCESS] Student record saved successfully! <<\n";
}

/**
 * 2. Display All Student Records
 */
void displayAllStudents() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\n>> No student records found. Add students first!\n";
        return;
    }

    Student s;
    int count = 0;

    cout << "\n================================================================================";
    cout << "\n                            STUDENT DIRECTORY LIST                              ";
    cout << "\n================================================================================\n";
    cout << left << setw(10) << "Roll No."
         << setw(22) << "Name"
         << setw(16) << "Course"
         << setw(10) << "Total"
         << setw(10) << "Percent"
         << setw(8)  << "Grade"
         << setw(6)  << "GPA" << endl;
    cout << "--------------------------------------------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        s.showTabularRow();
        count++;
    }
    cout << "--------------------------------------------------------------------------------\n";
    cout << " Total Students Registered: " << count << endl;
    cout << "================================================================================\n";
    inFile.close();
}

/**
 * 3. Multi-Criteria Search System
 */
void searchStudentMenu() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\n>> No student records found.\n";
        return;
    }

    cout << "\n--------------------------------------------------------";
    cout << "\n                 SEARCH STUDENT RECORDS                 ";
    cout << "\n--------------------------------------------------------";
    cout << "\n  1. Search by Roll Number";
    cout << "\n  2. Search by Student Name (Partial/Full match)";
    cout << "\n  3. Filter by Grade (e.g. A+, A, B, C, F)";
    cout << "\n  4. Back to Main Menu";
    cout << "\n--------------------------------------------------------";
    cout << "\n  Enter your search choice (1-4): ";

    int searchChoice;
    if (!(cin >> searchChoice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    Student s;
    bool found = false;

    switch (searchChoice) {
        case 1: {
            int roll;
            cout << "Enter Roll Number to Search: ";
            cin >> roll;
            while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
                if (s.getRollNumber() == roll) {
                    s.generateReportCard();
                    found = true;
                    break;
                }
            }
            break;
        }
        case 2: {
            string searchName;
            cout << "Enter Name to Search: ";
            cin.ignore();
            getline(cin, searchName);
            string searchUpper = toUpperString(searchName);

            cout << "\nSearch Results for \"" << searchName << "\":\n";
            cout << "--------------------------------------------------------------------------------\n";
            while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
                string studentUpper = toUpperString(s.getName());
                if (studentUpper.find(searchUpper) != string::npos) {
                    s.showTabularRow();
                    found = true;
                }
            }
            cout << "--------------------------------------------------------------------------------\n";
            break;
        }
        case 3: {
            string filterGrade;
            cout << "Enter Grade to Filter (A+, A, B, C, D, E, F): ";
            cin >> filterGrade;
            string filterUpper = toUpperString(filterGrade);

            cout << "\nStudents with Grade \"" << filterGrade << "\":\n";
            cout << "--------------------------------------------------------------------------------\n";
            while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
                if (toUpperString(s.getGrade()) == filterUpper) {
                    s.showTabularRow();
                    found = true;
                }
            }
            cout << "--------------------------------------------------------------------------------\n";
            break;
        }
        case 4:
            inFile.close();
            return;
        default:
            cout << ">> Invalid search option.\n";
            inFile.close();
            return;
    }

    inFile.close();
    if (!found) {
        cout << ">> No matching student records found.\n";
    }
}

/**
 * 4. Generate Report Card for a specific student
 */
void generateReportCardService() {
    int roll;
    cout << "\nEnter Student Roll Number to Generate Report Card: ";
    cin >> roll;

    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << ">> No records found.\n";
        return;
    }

    Student s;
    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        if (s.getRollNumber() == roll) {
            s.generateReportCard();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << ">> [ERROR] Student with Roll Number #" << roll << " not found!\n";
    }
}

/**
 * 5. Update Student Record
 */
void updateStudentRecord() {
    int roll;
    cout << "\n--------------------------------------------------------";
    cout << "\n                UPDATE STUDENT RECORD                   ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Roll Number of Student to Update: ";
    cin >> roll;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << ">> No records database found.\n";
        return;
    }

    Student s;
    bool found = false;
    while (file.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        if (s.getRollNumber() == roll) {
            found = true;
            s.updateDetails();

            // Seek back 1 record size and overwrite
            file.seekp(-static_cast<streamoff>(sizeof(Student)), ios::cur);
            file.write(reinterpret_cast<const char *>(&s), sizeof(Student));
            cout << "\n>> [SUCCESS] Student record updated successfully! <<\n";
            break;
        }
    }
    file.close();

    if (!found) {
        cout << ">> [ERROR] Student with Roll Number #" << roll << " not found!\n";
    }
}

/**
 * 6. Delete Student Record
 */
void deleteStudentRecord() {
    int roll;
    cout << "\n--------------------------------------------------------";
    cout << "\n                DELETE STUDENT RECORD                   ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Roll Number to Delete: ";
    cin >> roll;

    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << ">> No records database found.\n";
        return;
    }

    Student s;
    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        if (s.getRollNumber() == roll) {
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << ">> [ERROR] Student with Roll Number #" << roll << " not found!\n";
        return;
    }

    cout << "Are you sure you want to permanently delete Roll #" << roll << " (" << s.getName() << ")? (Y/N): ";
    char confirm;
    cin >> confirm;
    if (toupper(confirm) != 'Y') {
        cout << ">> Deletion cancelled.\n";
        return;
    }

    // Reorganize binary file via temporary file
    inFile.open(FILE_NAME, ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        if (s.getRollNumber() != roll) {
            outFile.write(reinterpret_cast<const char *>(&s), sizeof(Student));
        }
    }

    inFile.close();
    outFile.close();

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    cout << "\n>> [SUCCESS] Student record deleted successfully! <<\n";
}

/**
 * 7. Unique Feature: Classroom Analytics & Statistics Dashboard
 */
void classAnalyticsDashboard() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\n>> No student records available for analytics.\n";
        return;
    }

    Student s;
    int totalCount = 0;
    int passCount = 0;
    double totalPercentageSum = 0.0;

    Student topper;
    Student lowest;
    double maxPercentage = -1.0;
    double minPercentage = 101.0;

    while (inFile.read(reinterpret_cast<char *>(&s), sizeof(Student))) {
        totalCount++;
        totalPercentageSum += s.getPercentage();

        if (s.isPassed()) {
            passCount++;
        }

        if (s.getPercentage() > maxPercentage) {
            maxPercentage = s.getPercentage();
            topper = s;
        }

        if (s.getPercentage() < minPercentage) {
            minPercentage = s.getPercentage();
            lowest = s;
        }
    }
    inFile.close();

    if (totalCount == 0) {
        cout << "\n>> No students currently in record.\n";
        return;
    }

    double classAverage = totalPercentageSum / totalCount;
    double passPercentage = (static_cast<double>(passCount) / totalCount) * 100.0;

    cout << "\n+==============================================================+";
    cout << "\n|                 CLASSROOM ANALYTICS DASHBOARD                |";
    cout << "\n+==============================================================+";
    cout << "\n  Total Enrolled Students : " << totalCount;
    cout << "\n  Class Average Percentage: " << fixed << setprecision(2) << classAverage << "%";
    cout << "\n  Students Passed         : " << passCount << " / " << totalCount << " (" << passPercentage << "%)";
    cout << "\n  Students Failed         : " << (totalCount - passCount) << " / " << totalCount;
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  CLASS TOPPER (HIGHEST SCORER):";
    cout << "\n    Name       : " << topper.getName();
    cout << "\n    Roll No.   : " << topper.getRollNumber();
    cout << "\n    Course     : " << topper.getCourse();
    cout << "\n    Percentage : " << fixed << setprecision(2) << topper.getPercentage() << "% (Grade: " << topper.getGrade() << ")";
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  LOWEST SCORER:";
    cout << "\n    Name       : " << lowest.getName();
    cout << "\n    Roll No.   : " << lowest.getRollNumber();
    cout << "\n    Percentage : " << fixed << setprecision(2) << lowest.getPercentage() << "% (Grade: " << lowest.getGrade() << ")";
    cout << "\n+==============================================================+\n";
}

// ============================================================================
// MAIN MENU
// ============================================================================

int main() {
    int choice;

    do {
        cout << "\n========================================================";
        cout << "\n               STUDENT MANAGEMENT SYSTEM                ";
        cout << "\n========================================================";
        cout << "\n  [1] Add New Student Record";
        cout << "\n  [2] Display All Students (Master Directory)";
        cout << "\n  [3] Search Student (By Roll, Name, or Grade)";
        cout << "\n  [4] Generate Academic Report Card / Marksheet";
        cout << "\n  [5] Update Student Record";
        cout << "\n  [6] Delete Student Record";
        cout << "\n  [7] Classroom Analytics & Statistics Dashboard";
        cout << "\n  [8] Exit Application";
        cout << "\n--------------------------------------------------------";
        cout << "\n  Enter your choice (1-8): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n>> Invalid input! Please enter a number between 1 and 8.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addStudentRecord();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudentMenu();
                break;
            case 4:
                generateReportCardService();
                break;
            case 5:
                updateStudentRecord();
                break;
            case 6:
                deleteStudentRecord();
                break;
            case 7:
                classAnalyticsDashboard();
                break;
            case 8:
                cout << "\n========================================================";
                cout << "\n     Exiting Student Management System. Goodbye!        ";
                cout << "\n========================================================\n\n";
                break;
            default:
                cout << "\n>> Invalid choice! Please select between 1 and 8.\n";
        }
    } while (choice != 8);

    return 0;
}
