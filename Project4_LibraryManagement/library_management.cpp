/**
 * ============================================================================
 * Project: Library Management System
 * Language: C++ (Console-Based)
 * Concepts Demonstrated:
 *   1. Object-Oriented Programming (OOP)
 *      - Encapsulation & Data Hiding (Book, Member, and IssueRecord classes)
 *      - Modular Class Design & Constructors
 *   2. Persistent File Management (Binary stream I/O via fstream)
 *   3. Menu-Driven Architecture with Input Validation
 *   4. Unique Features:
 *      - Automated Overdue Fine & Late Fee Calculator
 *      - Digital Checkout & Return Slip Generator with timestamps
 *      - Member Borrowing Limit (Max 3 Books) & Active Loan Tracker
 *      - Multi-Attribute Search & Genre / Category Filtering
 *      - Comprehensive Library Inventory & Statistics Dashboard
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

// ============================================================================
// CONSTANTS & CONFIGURATION
// ============================================================================
const char BOOKS_FILE[]  = "books.dat";
const char MEMBERS_FILE[] = "members.dat";
const char ISSUES_FILE[] = "issues.dat";

const int MAX_BORROW_LIMIT = 3;       // Maximum books a member can borrow at once
const int BORROW_PERIOD_DAYS = 14;     // Standard lending period in days
const double DAILY_FINE_RATE = 5.0;    // Fine: Rs. 5.00 per day overdue

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

string toUpper(const string &str) {
    string res = str;
    transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

string getFormattedDate(int daysFromNow = 0) {
    time_t t = time(0) + (daysFromNow * 24 * 60 * 60);
    tm *ltm = localtime(&t);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%d-%b-%Y", ltm);
    return string(buffer);
}

// ============================================================================
// CLASS: Book (Encapsulated Book Entity)
// ============================================================================

class Book {
private:
    int bookId;
    char title[60];
    char author[50];
    char genre[30];
    int totalCopies;
    int availableCopies;

public:
    Book();

    void setupBook(int id);
    void showTabularRow() const;
    void showDetails() const;

    bool issueCopy();
    void returnCopy();

    // Getters
    int getBookId() const { return bookId; }
    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }
    const char* getGenre() const { return genre; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }
};

Book::Book() {
    bookId = 0;
    title[0] = '\0';
    author[0] = '\0';
    genre[0] = '\0';
    totalCopies = 0;
    availableCopies = 0;
}

void Book::setupBook(int id) {
    bookId = id;

    cout << "Enter Book Title: ";
    cin.ignore();
    cin.getline(title, 60);

    cout << "Enter Author Name: ";
    cin.getline(author, 50);

    cout << "Select / Enter Genre (e.g. Computer Science, Science, Fiction, History, Business): ";
    cin.getline(genre, 30);

    while (true) {
        cout << "Enter Total Number of Copies to Stock: ";
        if (cin >> totalCopies && totalCopies > 0) {
            availableCopies = totalCopies;
            break;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << ">> Invalid quantity! Must be at least 1.\n";
    }

    cout << "\n>> [SUCCESS] Book \"" << title << "\" (ID #" << bookId << ") registered into inventory! <<\n";
}

void Book::showTabularRow() const {
    cout << left << setw(8) << bookId
         << setw(26) << (strlen(title) > 24 ? string(title).substr(0, 22) + ".." : title)
         << setw(20) << (strlen(author) > 18 ? string(author).substr(0, 16) + ".." : author)
         << setw(16) << genre
         << setw(8) << totalCopies
         << setw(10) << availableCopies
         << (availableCopies > 0 ? "IN STOCK" : "BORROWED") << endl;
}

void Book::showDetails() const {
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n|                      BOOK DETAILS CARD                       |";
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  Book ID          : " << bookId;
    cout << "\n  Title            : " << title;
    cout << "\n  Author           : " << author;
    cout << "\n  Genre            : " << genre;
    cout << "\n  Total Stock      : " << totalCopies;
    cout << "\n  Available Copies : " << availableCopies;
    cout << "\n  Status           : " << (availableCopies > 0 ? "Available for Checkout" : "All Copies Currently Issued");
    cout << "\n+--------------------------------------------------------------+\n";
}

bool Book::issueCopy() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    return false;
}

void Book::returnCopy() {
    if (availableCopies < totalCopies) {
        availableCopies++;
    }
}

// ============================================================================
// CLASS: Member (Encapsulated Library Member Entity)
// ============================================================================

class Member {
private:
    int memberId;
    char name[50];
    char phone[16];
    int issuedCount;
    int issuedBookIds[MAX_BORROW_LIMIT];

public:
    Member();

    void setupMember(int id);
    void showTabularRow() const;
    void showDetails() const;

    bool canBorrow() const { return issuedCount < MAX_BORROW_LIMIT; }
    bool hasBorrowed(int bId) const;
    bool addIssuedBook(int bId);
    bool removeIssuedBook(int bId);

    // Getters
    int getMemberId() const { return memberId; }
    const char* getName() const { return name; }
    const char* getPhone() const { return phone; }
    int getIssuedCount() const { return issuedCount; }
};

Member::Member() {
    memberId = 0;
    name[0] = '\0';
    phone[0] = '\0';
    issuedCount = 0;
    for (int i = 0; i < MAX_BORROW_LIMIT; i++) {
        issuedBookIds[i] = 0;
    }
}

void Member::setupMember(int id) {
    memberId = id;

    cout << "Enter Member Full Name: ";
    cin.ignore();
    cin.getline(name, 50);

    cout << "Enter Contact Phone Number: ";
    cin.getline(phone, 16);

    cout << "\n>> [SUCCESS] Member \"" << name << "\" (ID #" << memberId << ") registered successfully! <<\n";
}

void Member::showTabularRow() const {
    cout << left << setw(10) << memberId
         << setw(26) << name
         << setw(16) << phone
         << issuedCount << " / " << MAX_BORROW_LIMIT << " books borrowed" << endl;
}

void Member::showDetails() const {
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n|                     MEMBER PROFILE CARD                      |";
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  Member ID        : " << memberId;
    cout << "\n  Full Name        : " << name;
    cout << "\n  Phone Number     : " << phone;
    cout << "\n  Active Loans     : " << issuedCount << " / " << MAX_BORROW_LIMIT;
    cout << "\n  Borrowed Book IDs: ";
    if (issuedCount == 0) {
        cout << "None";
    } else {
        for (int i = 0; i < issuedCount; i++) {
            cout << "#" << issuedBookIds[i] << " ";
        }
    }
    cout << "\n+--------------------------------------------------------------+\n";
}

bool Member::hasBorrowed(int bId) const {
    for (int i = 0; i < issuedCount; i++) {
        if (issuedBookIds[i] == bId) return true;
    }
    return false;
}

bool Member::addIssuedBook(int bId) {
    if (issuedCount < MAX_BORROW_LIMIT) {
        issuedBookIds[issuedCount++] = bId;
        return true;
    }
    return false;
}

bool Member::removeIssuedBook(int bId) {
    for (int i = 0; i < issuedCount; i++) {
        if (issuedBookIds[i] == bId) {
            for (int j = i; j < issuedCount - 1; j++) {
                issuedBookIds[j] = issuedBookIds[j + 1];
            }
            issuedBookIds[--issuedCount] = 0;
            return true;
        }
    }
    return false;
}

// ============================================================================
// CLASS: IssueRecord (Encapsulated Transaction Entity)
// ============================================================================

class IssueRecord {
private:
    int transactionId;
    int bookId;
    int memberId;
    char issueDate[20];
    char dueDate[20];
    char returnDate[20];
    double finePaid;
    bool isReturned;

public:
    IssueRecord();

    void createRecord(int transId, int bId, int mId);
    void markReturned(const string &retDate, double fine);

    // Getters
    int getTransactionId() const { return transactionId; }
    int getBookId() const { return bookId; }
    int getMemberId() const { return memberId; }
    const char* getIssueDate() const { return issueDate; }
    const char* getDueDate() const { return dueDate; }
    const char* getReturnDate() const { return returnDate; }
    double getFinePaid() const { return finePaid; }
    bool getIsReturned() const { return isReturned; }
};

IssueRecord::IssueRecord() {
    transactionId = 0;
    bookId = 0;
    memberId = 0;
    issueDate[0] = '\0';
    dueDate[0] = '\0';
    strcpy(returnDate, "NOT RETURNED");
    finePaid = 0.0;
    isReturned = false;
}

void IssueRecord::createRecord(int transId, int bId, int mId) {
    transactionId = transId;
    bookId = bId;
    memberId = mId;
    strncpy(issueDate, getFormattedDate(0).c_str(), 19);
    strncpy(dueDate, getFormattedDate(BORROW_PERIOD_DAYS).c_str(), 19);
    strcpy(returnDate, "ACTIVE LOAN");
    finePaid = 0.0;
    isReturned = false;
}

void IssueRecord::markReturned(const string &retDate, double fine) {
    strncpy(returnDate, retDate.c_str(), 19);
    finePaid = fine;
    isReturned = true;
}

// ============================================================================
// DIGITAL RECEIPT SLIP GENERATOR
// ============================================================================

void printCheckoutSlip(int transId, int bId, const string &title, int mId, const string &mName, const string &issueDt, const string &dueDt) {
    cout << "\n================================================================";
    cout << "\n                  LIBRARY CHECKOUT SLIP                         ";
    cout << "\n================================================================";
    cout << "\n  Transaction ID : #" << transId;
    cout << "\n  Issue Date     : " << issueDt;
    cout << "\n  Due Date       : " << dueDt << " (14 Days Lending Period)";
    cout << "\n----------------------------------------------------------------";
    cout << "\n  Member ID      : #" << mId << " - " << mName;
    cout << "\n  Book Issued    : #" << bId << " - " << title;
    cout << "\n----------------------------------------------------------------";
    cout << "\n  Note: Overdue fine of Rs. " << fixed << setprecision(2) << DAILY_FINE_RATE << "/day applies after due date.";
    cout << "\n================================================================\n";
}

void printReturnSlip(int transId, int bId, const string &title, int mId, const string &mName, int daysLate, double fine) {
    cout << "\n================================================================";
    cout << "\n                  LIBRARY RETURN RECEIPT                        ";
    cout << "\n================================================================";
    cout << "\n  Transaction ID : #" << transId;
    cout << "\n  Return Date    : " << getFormattedDate(0);
    cout << "\n  Member         : #" << mId << " - " << mName;
    cout << "\n  Book Returned  : #" << bId << " - " << title;
    cout << "\n----------------------------------------------------------------";
    cout << "\n  Overdue Days   : " << daysLate << " Day(s)";
    cout << "\n  Late Fine Paid : Rs. " << fixed << setprecision(2) << fine;
    cout << "\n  Status         : RETURN PROCESSED & STOCK REPLENISHED";
    cout << "\n================================================================\n";
}

// ============================================================================
// PERSISTENCE SERVICES: BOOKS MANAGEMENT
// ============================================================================

bool bookExists(int id) {
    Book b;
    ifstream f(BOOKS_FILE, ios::binary);
    if (!f) return false;
    while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if (b.getBookId() == id) {
            f.close();
            return true;
        }
    }
    f.close();
    return false;
}

void addBookService() {
    cout << "\n--------------------------------------------------------";
    cout << "\n                   ADD NEW BOOK ENTRY                   ";
    cout << "\n--------------------------------------------------------\n";
    int id;
    while (true) {
        cout << "Enter Unique Book ID (Positive integer): ";
        if (cin >> id && id > 0) break;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << ">> Invalid input! Must be a positive integer.\n";
    }

    if (bookExists(id)) {
        cout << "\n>> [ERROR] Book with ID #" << id << " already exists!\n";
        return;
    }

    Book b;
    b.setupBook(id);

    ofstream f(BOOKS_FILE, ios::binary | ios::app);
    f.write(reinterpret_cast<const char*>(&b), sizeof(Book));
    f.close();
}

void displayInventoryService() {
    ifstream f(BOOKS_FILE, ios::binary);
    if (!f) {
        cout << "\n>> No books registered yet. Add books first!\n";
        return;
    }

    cout << "\n=================================================================================================";
    cout << "\n                                   LIBRARY BOOK INVENTORY                                        ";
    cout << "\n=================================================================================================\n";
    cout << left << setw(8) << "ID"
         << setw(26) << "Title"
         << setw(20) << "Author"
         << setw(16) << "Genre"
         << setw(8)  << "Total"
         << setw(10) << "Available"
         << "Status" << endl;
    cout << "-------------------------------------------------------------------------------------------------\n";

    Book b;
    int count = 0;
    while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        b.showTabularRow();
        count++;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << " Total Distinct Titles: " << count << endl;
    cout << "=================================================================================================\n";
    f.close();
}

void searchBookService() {
    ifstream f(BOOKS_FILE, ios::binary);
    if (!f) {
        cout << "\n>> No books found in records.\n";
        return;
    }

    cout << "\n--------------------------------------------------------";
    cout << "\n                  SEARCH BOOK CATALOG                   ";
    cout << "\n--------------------------------------------------------";
    cout << "\n  1. Search by Book Title";
    cout << "\n  2. Search by Author Name";
    cout << "\n  3. Search by Book ID";
    cout << "\n  4. Filter Books by Genre / Category";
    cout << "\n  5. Back to Main Menu";
    cout << "\n--------------------------------------------------------";
    cout << "\n  Enter your selection (1-5): ";

    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    Book b;
    bool found = false;

    switch (choice) {
        case 1: {
            string query;
            cout << "Enter Book Title to Search: ";
            cin.ignore();
            getline(cin, query);
            string qUpper = toUpper(query);

            cout << "\nSearch Results matching Title \"" << query << "\":\n";
            cout << "-------------------------------------------------------------------------------------------------\n";
            while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
                if (toUpper(b.getTitle()).find(qUpper) != string::npos) {
                    b.showTabularRow();
                    found = true;
                }
            }
            cout << "-------------------------------------------------------------------------------------------------\n";
            break;
        }
        case 2: {
            string query;
            cout << "Enter Author Name to Search: ";
            cin.ignore();
            getline(cin, query);
            string qUpper = toUpper(query);

            cout << "\nSearch Results matching Author \"" << query << "\":\n";
            cout << "-------------------------------------------------------------------------------------------------\n";
            while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
                if (toUpper(b.getAuthor()).find(qUpper) != string::npos) {
                    b.showTabularRow();
                    found = true;
                }
            }
            cout << "-------------------------------------------------------------------------------------------------\n";
            break;
        }
        case 3: {
            int id;
            cout << "Enter Book ID to Search: ";
            cin >> id;
            while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
                if (b.getBookId() == id) {
                    b.showDetails();
                    found = true;
                    break;
                }
            }
            break;
        }
        case 4: {
            string genreQuery;
            cout << "Enter Genre to Filter (e.g. Science, Fiction, History, Computer Science): ";
            cin.ignore();
            getline(cin, genreQuery);
            string gUpper = toUpper(genreQuery);

            cout << "\nBooks in Genre \"" << genreQuery << "\":\n";
            cout << "-------------------------------------------------------------------------------------------------\n";
            while (f.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
                if (toUpper(b.getGenre()).find(gUpper) != string::npos) {
                    b.showTabularRow();
                    found = true;
                }
            }
            cout << "-------------------------------------------------------------------------------------------------\n";
            break;
        }
        case 5:
            f.close();
            return;
        default:
            cout << ">> Invalid search option.\n";
            f.close();
            return;
    }

    f.close();
    if (!found) {
        cout << ">> No matching books found in inventory.\n";
    }
}

// ============================================================================
// PERSISTENCE SERVICES: MEMBERS MANAGEMENT
// ============================================================================

bool memberExists(int id) {
    Member m;
    ifstream f(MEMBERS_FILE, ios::binary);
    if (!f) return false;
    while (f.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        if (m.getMemberId() == id) {
            f.close();
            return true;
        }
    }
    f.close();
    return false;
}

void registerMemberService() {
    cout << "\n--------------------------------------------------------";
    cout << "\n               REGISTER NEW LIBRARY MEMBER              ";
    cout << "\n--------------------------------------------------------\n";
    int id;
    while (true) {
        cout << "Enter Unique Member ID (Positive integer): ";
        if (cin >> id && id > 0) break;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << ">> Invalid input! Must be a positive integer.\n";
    }

    if (memberExists(id)) {
        cout << "\n>> [ERROR] Member with ID #" << id << " already exists!\n";
        return;
    }

    Member m;
    m.setupMember(id);

    ofstream f(MEMBERS_FILE, ios::binary | ios::app);
    f.write(reinterpret_cast<const char*>(&m), sizeof(Member));
    f.close();
}

void displayMembersService() {
    ifstream f(MEMBERS_FILE, ios::binary);
    if (!f) {
        cout << "\n>> No registered members found.\n";
        return;
    }

    cout << "\n================================================================================";
    cout << "\n                          REGISTERED LIBRARY MEMBERS                            ";
    cout << "\n================================================================================\n";
    cout << left << setw(10) << "Member ID"
         << setw(26) << "Name"
         << setw(16) << "Phone"
         << "Loan Status" << endl;
    cout << "--------------------------------------------------------------------------------\n";

    Member m;
    int count = 0;
    while (f.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        m.showTabularRow();
        count++;
    }
    cout << "--------------------------------------------------------------------------------\n";
    cout << " Total Registered Members: " << count << endl;
    cout << "================================================================================\n";
    f.close();
}

void viewMemberDetailsService() {
    int id;
    cout << "\nEnter Member ID to view active loans and profile: ";
    cin >> id;

    ifstream f(MEMBERS_FILE, ios::binary);
    if (!f) {
        cout << ">> No member database found.\n";
        return;
    }

    Member m;
    bool found = false;
    while (f.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        if (m.getMemberId() == id) {
            m.showDetails();
            found = true;
            break;
        }
    }
    f.close();

    if (!found) {
        cout << ">> [ERROR] Member ID #" << id << " not found!\n";
    }
}

// ============================================================================
// CIRCULATION SERVICES: ISSUE & RETURN WITH FINE CALCULATION
// ============================================================================

int getNextTransactionId() {
    ifstream f(ISSUES_FILE, ios::binary);
    if (!f) return 1001;

    IssueRecord rec;
    int maxId = 1000;
    while (f.read(reinterpret_cast<char*>(&rec), sizeof(IssueRecord))) {
        if (rec.getTransactionId() > maxId) {
            maxId = rec.getTransactionId();
        }
    }
    f.close();
    return maxId + 1;
}

/**
 * Check Out / Issue Book to Member
 */
void issueBookService() {
    cout << "\n--------------------------------------------------------";
    cout << "\n                    ISSUE BOOK TO MEMBER                ";
    cout << "\n--------------------------------------------------------\n";

    int mId, bId;
    cout << "Enter Member ID : ";
    cin >> mId;
    cout << "Enter Book ID   : ";
    cin >> bId;

    // 1. Verify Member
    fstream mFile(MEMBERS_FILE, ios::binary | ios::in | ios::out);
    if (!mFile) {
        cout << ">> No members database found.\n";
        return;
    }

    Member member;
    bool mFound = false;
    streampos mPos;
    while (mFile.read(reinterpret_cast<char*>(&member), sizeof(Member))) {
        if (member.getMemberId() == mId) {
            mFound = true;
            mPos = mFile.tellg() - static_cast<streamoff>(sizeof(Member));
            break;
        }
    }

    if (!mFound) {
        cout << ">> [ERROR] Member ID #" << mId << " does not exist!\n";
        mFile.close();
        return;
    }

    if (!member.canBorrow()) {
        cout << "\n>> [DECLINED] Member has reached the maximum borrowing limit (" 
             << MAX_BORROW_LIMIT << " books)!\n";
        cout << ">> Please return an existing borrowed book before issuing a new one.\n";
        mFile.close();
        return;
    }

    if (member.hasBorrowed(bId)) {
        cout << "\n>> [DECLINED] Member has already checked out a copy of this book (#" << bId << ")!\n";
        mFile.close();
        return;
    }

    // 2. Verify Book Stock
    fstream bFile(BOOKS_FILE, ios::binary | ios::in | ios::out);
    if (!bFile) {
        cout << ">> No books database found.\n";
        mFile.close();
        return;
    }

    Book book;
    bool bFound = false;
    streampos bPos;
    while (bFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.getBookId() == bId) {
            bFound = true;
            bPos = bFile.tellg() - static_cast<streamoff>(sizeof(Book));
            break;
        }
    }

    if (!bFound) {
        cout << ">> [ERROR] Book ID #" << bId << " does not exist in the library catalog!\n";
        mFile.close();
        bFile.close();
        return;
    }

    if (book.getAvailableCopies() <= 0) {
        cout << "\n>> [OUT OF STOCK] All copies of \"" << book.getTitle() << "\" are currently issued.\n";
        mFile.close();
        bFile.close();
        return;
    }

    // 3. Process Transaction
    book.issueCopy();
    member.addIssuedBook(bId);

    // Save updated book record
    bFile.seekp(bPos);
    bFile.write(reinterpret_cast<const char*>(&book), sizeof(Book));
    bFile.close();

    // Save updated member record
    mFile.seekp(mPos);
    mFile.write(reinterpret_cast<const char*>(&member), sizeof(Member));
    mFile.close();

    // 4. Create Issue Record
    int transId = getNextTransactionId();
    IssueRecord rec;
    rec.createRecord(transId, bId, mId);

    ofstream iFile(ISSUES_FILE, ios::binary | ios::app);
    iFile.write(reinterpret_cast<const char*>(&rec), sizeof(IssueRecord));
    iFile.close();

    // 5. Print Digital Checkout Slip
    cout << "\n>> [SUCCESS] Book issued successfully! <<\n";
    printCheckoutSlip(transId, bId, book.getTitle(), mId, member.getName(), rec.getIssueDate(), rec.getDueDate());
}

/**
 * Return Book with Overdue Late Fee Calculation
 */
void returnBookService() {
    cout << "\n--------------------------------------------------------";
    cout << "\n              RETURN BORROWED BOOK & SETTLE             ";
    cout << "\n--------------------------------------------------------\n";

    int mId, bId;
    cout << "Enter Member ID : ";
    cin >> mId;
    cout << "Enter Book ID   : ";
    cin >> bId;

    // 1. Locate Member
    fstream mFile(MEMBERS_FILE, ios::binary | ios::in | ios::out);
    if (!mFile) {
        cout << ">> No members database found.\n";
        return;
    }

    Member member;
    bool mFound = false;
    streampos mPos;
    while (mFile.read(reinterpret_cast<char*>(&member), sizeof(Member))) {
        if (member.getMemberId() == mId) {
            mFound = true;
            mPos = mFile.tellg() - static_cast<streamoff>(sizeof(Member));
            break;
        }
    }

    if (!mFound) {
        cout << ">> [ERROR] Member ID #" << mId << " not found!\n";
        mFile.close();
        return;
    }

    if (!member.hasBorrowed(bId)) {
        cout << ">> [ERROR] Member #" << mId << " does not have Book #" << bId << " checked out!\n";
        mFile.close();
        return;
    }

    // 2. Locate Active Issue Transaction Record
    fstream iFile(ISSUES_FILE, ios::binary | ios::in | ios::out);
    IssueRecord rec;
    bool recFound = false;
    streampos iPos;
    while (iFile.read(reinterpret_cast<char*>(&rec), sizeof(IssueRecord))) {
        if (rec.getMemberId() == mId && rec.getBookId() == bId && !rec.getIsReturned()) {
            recFound = true;
            iPos = iFile.tellg() - static_cast<streamoff>(sizeof(IssueRecord));
            break;
        }
    }

    // 3. Locate Book Record
    fstream bFile(BOOKS_FILE, ios::binary | ios::in | ios::out);
    Book book;
    bool bFound = false;
    streampos bPos;
    while (bFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.getBookId() == bId) {
            bFound = true;
            bPos = bFile.tellg() - static_cast<streamoff>(sizeof(Book));
            break;
        }
    }

    // 4. Overdue Fine Calculation Engine
    cout << "\n>> Book Due Date on Record: " << rec.getDueDate() << endl;
    cout << "Is this return past the 14-day due date? (Y/N): ";
    char lateChoice;
    cin >> lateChoice;

    int daysLate = 0;
    double fine = 0.0;
    if (toupper(lateChoice) == 'Y') {
        while (true) {
            cout << "Enter number of days overdue: ";
            if (cin >> daysLate && daysLate >= 0) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ">> Invalid input! Enter 0 or positive number.\n";
        }
        fine = daysLate * DAILY_FINE_RATE;
        cout << "\n>> Late Fee Assessed: " << daysLate << " days @ Rs. " 
             << fixed << setprecision(2) << DAILY_FINE_RATE << "/day = Rs. " << fine << endl;
    }

    // Update records
    book.returnCopy();
    member.removeIssuedBook(bId);
    rec.markReturned(getFormattedDate(0), fine);

    // Write back Book
    if (bFound) {
        bFile.seekp(bPos);
        bFile.write(reinterpret_cast<const char*>(&book), sizeof(Book));
    }
    bFile.close();

    // Write back Member
    mFile.seekp(mPos);
    mFile.write(reinterpret_cast<const char*>(&member), sizeof(Member));
    mFile.close();

    // Write back Issue Record
    if (recFound) {
        iFile.seekp(iPos);
        iFile.write(reinterpret_cast<const char*>(&rec), sizeof(IssueRecord));
    }
    iFile.close();

    // 5. Print Return Slip
    cout << "\n>> [SUCCESS] Return processed and book returned to inventory! <<\n";
    printReturnSlip(rec.getTransactionId(), bId, book.getTitle(), mId, member.getName(), daysLate, fine);
}

// ============================================================================
// ANALYTICS & INVENTORY DASHBOARD
// ============================================================================

void libraryDashboardService() {
    int totalTitles = 0;
    int totalStock = 0;
    int availableStock = 0;

    ifstream bFile(BOOKS_FILE, ios::binary);
    Book b;
    if (bFile) {
        while (bFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
            totalTitles++;
            totalStock += b.getTotalCopies();
            availableStock += b.getAvailableCopies();
        }
        bFile.close();
    }

    int totalMembers = 0;
    int activeBorrowers = 0;
    ifstream mFile(MEMBERS_FILE, ios::binary);
    Member m;
    if (mFile) {
        while (mFile.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
            totalMembers++;
            if (m.getIssuedCount() > 0) activeBorrowers++;
        }
        mFile.close();
    }

    int totalIssuesCount = 0;
    int activeLoansCount = 0;
    double totalFinesCollected = 0.0;
    ifstream iFile(ISSUES_FILE, ios::binary);
    IssueRecord rec;
    if (iFile) {
        while (iFile.read(reinterpret_cast<char*>(&rec), sizeof(IssueRecord))) {
            totalIssuesCount++;
            totalFinesCollected += rec.getFinePaid();
            if (!rec.getIsReturned()) activeLoansCount++;
        }
        iFile.close();
    }

    cout << "\n+==============================================================+";
    cout << "\n|                 LIBRARY MANAGEMENT DASHBOARD                 |";
    cout << "\n+==============================================================+";
    cout << "\n  CATALOG & INVENTORY:";
    cout << "\n    Distinct Book Titles  : " << totalTitles;
    cout << "\n    Total Physical Copies : " << totalStock;
    cout << "\n    Copies on Shelves     : " << availableStock;
    cout << "\n    Copies on Loan        : " << (totalStock - availableStock);
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  MEMBERSHIP & CIRCULATION:";
    cout << "\n    Total Registered Members: " << totalMembers;
    cout << "\n    Members with Active Loans: " << activeBorrowers;
    cout << "\n    Total Loans Processed   : " << totalIssuesCount;
    cout << "\n    Currently Active Loans  : " << activeLoansCount;
    cout << "\n+--------------------------------------------------------------+";
    cout << "\n  FINANCIAL REVENUE:";
    cout << "\n    Total Overdue Fines Paid: Rs. " << fixed << setprecision(2) << totalFinesCollected;
    cout << "\n+==============================================================+\n";
}

// ============================================================================
// MAIN APPLICATION MENU
// ============================================================================

int main() {
    int choice;

    do {
        cout << "\n========================================================";
        cout << "\n               LIBRARY MANAGEMENT SYSTEM                ";
        cout << "\n========================================================";
        cout << "\n  --- BOOK MANAGEMENT ---";
        cout << "\n  [1] Add New Book to Inventory";
        cout << "\n  [2] View Entire Book Catalog";
        cout << "\n  [3] Search Books (Title / Author / ID / Genre)";
        cout << "\n";
        cout << "\n  --- CIRCULATION & DESK OPERATIONS ---";
        cout << "\n  [4] Issue Book (Checkout with Receipt)";
        cout << "\n  [5] Return Book (with Overdue Fine Calculation)";
        cout << "\n";
        cout << "\n  --- MEMBER MANAGEMENT ---";
        cout << "\n  [6] Register New Library Member";
        cout << "\n  [7] View All Members";
        cout << "\n  [8] View Member Profile & Active Loans";
        cout << "\n";
        cout << "\n  --- ANALYTICS ---";
        cout << "\n  [9] Library Statistics & Inventory Dashboard";
        cout << "\n  [10] Exit System";
        cout << "\n--------------------------------------------------------";
        cout << "\n  Enter your selection (1-10): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n>> Invalid input! Please enter a number between 1 and 10.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addBookService();
                break;
            case 2:
                displayInventoryService();
                break;
            case 3:
                searchBookService();
                break;
            case 4:
                issueBookService();
                break;
            case 5:
                returnBookService();
                break;
            case 6:
                registerMemberService();
                break;
            case 7:
                displayMembersService();
                break;
            case 8:
                viewMemberDetailsService();
                break;
            case 9:
                libraryDashboardService();
                break;
            case 10:
                cout << "\n========================================================";
                cout << "\n    Exiting Library Management System. Have a good day! ";
                cout << "\n========================================================\n\n";
                break;
            default:
                cout << "\n>> Invalid selection! Please choose between 1 and 10.\n";
        }
    } while (choice != 10);

    return 0;
}
