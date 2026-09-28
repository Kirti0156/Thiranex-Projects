/**
 * ============================================================================
 * Project: Secure Bank Management System
 * Language: C++ (Console-Based)
 * Concepts Demonstrated:
 *   1. Object-Oriented Programming (OOP)
 *      - Encapsulation & Data Hiding (private fields, public interfaces)
 *      - Constructors and Member Functions
 *   2. Secure File Handling (Binary stream I/O via fstream)
 *   3. Security & Validation (Masked PIN authentication, Minimum balance rules)
 *   4. Advanced Banking Features (Fund Transfer, Interest Calculation, Receipts)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <conio.h>    // For _getch() masked PIN input
#include <ctime>      // For timestamped transaction receipts
#include <cctype>     // For toupper()

using namespace std;

// ============================================================================
// CONSTANTS & RULES
// ============================================================================
const char FILE_NAME[] = "bank_records.dat";
const double MIN_SAVINGS_BALANCE = 500.0;
const double MIN_CURRENT_BALANCE = 1000.0;
const double SAVINGS_ANNUAL_INTEREST_RATE = 4.0; // 4% per annum

// ============================================================================
// HELPER FUNCTIONS (UTILITIES)
// ============================================================================

/**
 * Reads a 4-digit PIN while masking characters with '*' for privacy.
 * Supports backspace deletion.
 */
string inputMaskedPin() {
    string pin = "";
    char ch;
    while (true) {
        ch = _getch();
        // Enter key pressed
        if (ch == 13) {
            if (pin.length() == 4) break;
            else continue; // require full 4 digits
        }
        // Backspace key
        else if (ch == 8) {
            if (!pin.empty()) {
                pin.pop_back();
                cout << "\b \b";
            }
        }
        // Digits only
        else if (isdigit(ch) && pin.length() < 4) {
            pin.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return pin;
}

/**
 * Returns the current date and time formatted as a string.
 */
string getCurrentTimestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%d-%b-%Y %I:%M %p", ltm);
    return string(buffer);
}

// ============================================================================
// OBJECT-ORIENTED CLASS DESIGN: BankAccount
// ============================================================================

class BankAccount {
private:
    // Encapsulated data members (Data Hiding)
    int accountNumber;
    char holderName[50];
    char accountType;        // 'S' = Savings, 'C' = Current
    double balance;
    char pin[5];             // 4-digit PIN + null terminator

public:
    // Default Constructor
    BankAccount() {
        accountNumber = 0;
        holderName[0] = '\0';
        accountType = 'S';
        balance = 0.0;
        strcpy(pin, "0000");
    }

    // Member function to initialize an account
    bool setupAccount(int accNum);

    // Displays full account details card
    void showAccountDetails() const;

    // Displays a formatted single-line record for tabular reports
    void showTabularRecord() const;

    // Transaction Operations
    void depositAmount(double amount);
    bool withdrawAmount(double amount);

    // PIN Authentication
    bool verifyPin(const string &enteredPin) const {
        return strcmp(pin, enteredPin.c_str()) == 0;
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    char getAccountType() const { return accountType; }
    const char* getHolderName() const { return holderName; }

    // Interest preview for Savings accounts
    void displayInterestPreview() const;
};

// Member Function Implementations
bool BankAccount::setupAccount(int accNum) {
    accountNumber = accNum;

    cout << "\nEnter Account Holder Full Name: ";
    cin.ignore();
    cin.getline(holderName, 50);

    // Validate account type
    while (true) {
        cout << "Enter Account Type (S for Savings / C for Current): ";
        cin >> accountType;
        accountType = toupper(accountType);
        if (accountType == 'S' || accountType == 'C') break;
        cout << ">> Invalid type! Please enter 'S' or 'C'.\n";
    }

    // Determine minimum initial balance based on type
    double minDeposit = (accountType == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE;
    while (true) {
        cout << "Enter Initial Deposit (Minimum Rs. " << fixed << setprecision(2) << minDeposit << "): Rs. ";
        cin >> balance;
        if (balance >= minDeposit) break;
        cout << ">> Amount must be at least Rs. " << minDeposit << " for this account type!\n";
    }

    // Setup 4-Digit Security PIN
    cout << "Create a 4-Digit Security PIN (numbers only): ";
    string newPin = inputMaskedPin();
    strncpy(pin, newPin.c_str(), 4);
    pin[4] = '\0';

    cout << "\n>> [SUCCESS] Account #" << accountNumber << " has been created successfully! <<\n";
    return true;
}

void BankAccount::showAccountDetails() const {
    cout << "\n+------------------------------------------------------+";
    cout << "\n|                 ACCOUNT SUMMARY CARD                 |";
    cout << "\n+------------------------------------------------------+";
    cout << "\n  Account Number : " << accountNumber;
    cout << "\n  Holder Name    : " << holderName;
    cout << "\n  Account Type   : " << (accountType == 'S' ? "Savings Account" : "Current Account");
    cout << "\n  Current Balance: Rs. " << fixed << setprecision(2) << balance;
    cout << "\n  Min. Required  : Rs. " << (accountType == 'S' ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE);
    cout << "\n+------------------------------------------------------+\n";
}

void BankAccount::showTabularRecord() const {
    cout << left << setw(10) << accountNumber
         << setw(24) << holderName
         << setw(12) << (accountType == 'S' ? "Savings" : "Current")
         << "Rs. " << fixed << setprecision(2) << setw(12) << balance << endl;
}

void BankAccount::depositAmount(double amount) {
    balance += amount;
}

bool BankAccount::withdrawAmount(double amount) {
    double minAllowed = (accountType == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE;
    if (balance - amount < minAllowed) {
        cout << "\n>> [DECLINED] Transaction would violate minimum balance requirement!";
        cout << "\n>> Minimum balance required to maintain: Rs. " << fixed << setprecision(2) << minAllowed;
        cout << "\n>> Maximum withdrawable amount now     : Rs. " << fixed << setprecision(2) << (balance - minAllowed) << "\n";
        return false;
    }
    balance -= amount;
    return true;
}

void BankAccount::displayInterestPreview() const {
    if (accountType != 'S') {
        cout << "\n>> Note: Interest preview is only applicable to Savings Accounts.\n";
        return;
    }
    double annualInterest = (balance * SAVINGS_ANNUAL_INTEREST_RATE) / 100.0;
    cout << "\n+------------------------------------------------------+";
    cout << "\n|             SAVINGS INTEREST PROJECTION              |";
    cout << "\n+------------------------------------------------------+";
    cout << "\n  Current Balance  : Rs. " << fixed << setprecision(2) << balance;
    cout << "\n  Interest Rate    : " << SAVINGS_ANNUAL_INTEREST_RATE << "% p.a.";
    cout << "\n  Projected Return : Rs. " << annualInterest << " / year";
    cout << "\n  Est. End Balance : Rs. " << (balance + annualInterest);
    cout << "\n+------------------------------------------------------+\n";
}

// ============================================================================
// RECEIPT GENERATOR
// ============================================================================

void printTransactionReceipt(const string &type, int accNo, const string &name, double amount, double newBalance) {
    cout << "\n========================================================";
    cout << "\n                 TRANSACTION RECEIPT                    ";
    cout << "\n========================================================";
    cout << "\n  Date & Time : " << getCurrentTimestamp();
    cout << "\n  Transaction : " << type;
    cout << "\n  Account No  : " << accNo;
    cout << "\n  Holder Name : " << name;
    cout << "\n  Amount      : Rs. " << fixed << setprecision(2) << amount;
    cout << "\n  New Balance : Rs. " << fixed << setprecision(2) << newBalance;
    cout << "\n  Status      : SUCCESSFUL";
    cout << "\n========================================================\n";
}

// ============================================================================
// FILE MANAGEMENT OPERATIONS (PERSISTENCE LAYER)
// ============================================================================

/**
 * Checks if an account number already exists in the binary file.
 */
bool accountExists(int accNum) {
    BankAccount temp;
    ifstream file(FILE_NAME, ios::binary);
    if (!file) return false;

    while (file.read(reinterpret_cast<char *>(&temp), sizeof(BankAccount))) {
        if (temp.getAccountNumber() == accNum) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/**
 * Creates and appends a new account record into the binary file.
 */
void createAccountService() {
    int accNum;
    cout << "\n--------------------------------------------------------";
    cout << "\n               OPEN NEW BANK ACCOUNT                    ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter New Account Number (Positive integer): ";
    cin >> accNum;

    if (accNum <= 0) {
        cout << ">> Invalid account number! Must be greater than 0.\n";
        return;
    }

    if (accountExists(accNum)) {
        cout << ">> [ERROR] An account with Account Number #" << accNum << " already exists!\n";
        return;
    }

    BankAccount newAcc;
    newAcc.setupAccount(accNum);

    // Save to binary file (Append mode)
    ofstream outFile(FILE_NAME, ios::binary | ios::app);
    if (!outFile) {
        cout << ">> [ERROR] Could not open database file for writing.\n";
        return;
    }
    outFile.write(reinterpret_cast<const char *>(&newAcc), sizeof(BankAccount));
    outFile.close();
}

/**
 * Authenticates user PIN and displays balance & details.
 */
void balanceInquiryService() {
    int accNum;
    cout << "\n--------------------------------------------------------";
    cout << "\n                 BALANCE INQUIRY                        ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Account Number: ";
    cin >> accNum;

    BankAccount acc;
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << ">> No accounts found on record.\n";
        return;
    }

    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == accNum) {
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << ">> [ERROR] Account #" << accNum << " not found!\n";
        return;
    }

    // Authenticate with PIN
    cout << "Enter 4-Digit Security PIN: ";
    string enteredPin = inputMaskedPin();
    if (!acc.verifyPin(enteredPin)) {
        cout << ">> [ACCESS DENIED] Incorrect PIN entered!\n";
        return;
    }

    acc.showAccountDetails();
    if (acc.getAccountType() == 'S') {
        char seeInterest;
        cout << "Would you like to view annual interest projection? (Y/N): ";
        cin >> seeInterest;
        if (toupper(seeInterest) == 'Y') {
            acc.displayInterestPreview();
        }
    }
}

/**
 * Handles Cash Deposit into an account.
 */
void depositService() {
    int accNum;
    cout << "\n--------------------------------------------------------";
    cout << "\n                    CASH DEPOSIT                        ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Account Number: ";
    cin >> accNum;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << ">> No accounts found on record.\n";
        return;
    }

    BankAccount acc;
    bool found = false;
    while (file.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == accNum) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << ">> [ERROR] Account #" << accNum << " not found!\n";
        file.close();
        return;
    }

    cout << "Account Holder: " << acc.getHolderName() << endl;
    double amount;
    cout << "Enter Amount to Deposit: Rs. ";
    cin >> amount;

    if (amount <= 0) {
        cout << ">> Deposit amount must be positive!\n";
        file.close();
        return;
    }

    acc.depositAmount(amount);

    // Move file write pointer back by 1 record size to overwrite in place
    file.seekp(-static_cast<streamoff>(sizeof(BankAccount)), ios::cur);
    file.write(reinterpret_cast<const char *>(&acc), sizeof(BankAccount));
    file.close();

    printTransactionReceipt("CASH DEPOSIT", acc.getAccountNumber(), acc.getHolderName(), amount, acc.getBalance());
}

/**
 * Handles Cash Withdrawal with PIN verification and minimum balance check.
 */
void withdrawalService() {
    int accNum;
    cout << "\n--------------------------------------------------------";
    cout << "\n                  CASH WITHDRAWAL                       ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Account Number: ";
    cin >> accNum;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << ">> No accounts found on record.\n";
        return;
    }

    BankAccount acc;
    bool found = false;
    while (file.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == accNum) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << ">> [ERROR] Account #" << accNum << " not found!\n";
        file.close();
        return;
    }

    // Verify PIN before proceeding with withdrawal
    cout << "Enter 4-Digit Security PIN: ";
    string enteredPin = inputMaskedPin();
    if (!acc.verifyPin(enteredPin)) {
        cout << ">> [ACCESS DENIED] Incorrect PIN entered!\n";
        file.close();
        return;
    }

    double amount;
    cout << "Current Available Balance: Rs. " << fixed << setprecision(2) << acc.getBalance() << endl;
    cout << "Enter Amount to Withdraw: Rs. ";
    cin >> amount;

    if (amount <= 0) {
        cout << ">> Withdrawal amount must be greater than 0!\n";
        file.close();
        return;
    }

    if (!acc.withdrawAmount(amount)) {
        file.close();
        return;
    }

    // Overwrite updated record in file
    file.seekp(-static_cast<streamoff>(sizeof(BankAccount)), ios::cur);
    file.write(reinterpret_cast<const char *>(&acc), sizeof(BankAccount));
    file.close();

    printTransactionReceipt("CASH WITHDRAWAL", acc.getAccountNumber(), acc.getHolderName(), amount, acc.getBalance());
}

/**
 * Unique Feature: Atomic Fund Transfer from Sender to Receiver.
 */
void fundTransferService() {
    int senderAccNo, receiverAccNo;
    cout << "\n--------------------------------------------------------";
    cout << "\n               INTER-ACCOUNT FUND TRANSFER              ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Sender Account Number  : ";
    cin >> senderAccNo;
    cout << "Enter Receiver Account Number: ";
    cin >> receiverAccNo;

    if (senderAccNo == receiverAccNo) {
        cout << ">> [ERROR] Sender and Receiver accounts cannot be identical!\n";
        return;
    }

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << ">> No accounts found on record.\n";
        return;
    }

    BankAccount sender, receiver, temp;
    streampos senderPos = -1, receiverPos = -1;

    while (file.read(reinterpret_cast<char *>(&temp), sizeof(BankAccount))) {
        if (temp.getAccountNumber() == senderAccNo) {
            sender = temp;
            senderPos = file.tellg() - static_cast<streamoff>(sizeof(BankAccount));
        } else if (temp.getAccountNumber() == receiverAccNo) {
            receiver = temp;
            receiverPos = file.tellg() - static_cast<streamoff>(sizeof(BankAccount));
        }
    }

    if (senderPos == -1) {
        cout << ">> [ERROR] Sender account #" << senderAccNo << " does not exist!\n";
        file.close();
        return;
    }
    if (receiverPos == -1) {
        cout << ">> [ERROR] Receiver account #" << receiverAccNo << " does not exist!\n";
        file.close();
        return;
    }

    // Authenticate sender
    cout << "Sender: " << sender.getHolderName() << endl;
    cout << "Enter Sender's 4-Digit Security PIN: ";
    string enteredPin = inputMaskedPin();
    if (!sender.verifyPin(enteredPin)) {
        cout << ">> [ACCESS DENIED] Incorrect Sender PIN!\n";
        file.close();
        return;
    }

    double transferAmount;
    cout << "Enter Amount to Transfer to " << receiver.getHolderName() << ": Rs. ";
    cin >> transferAmount;

    if (transferAmount <= 0) {
        cout << ">> Transfer amount must be positive!\n";
        file.close();
        return;
    }

    if (!sender.withdrawAmount(transferAmount)) {
        file.close();
        return;
    }

    // Credit receiver
    receiver.depositAmount(transferAmount);

    // Save sender update
    file.seekp(senderPos);
    file.write(reinterpret_cast<const char *>(&sender), sizeof(BankAccount));

    // Save receiver update
    file.seekp(receiverPos);
    file.write(reinterpret_cast<const char *>(&receiver), sizeof(BankAccount));
    file.close();

    cout << "\n>> [SUCCESS] Fund Transfer Completed Successfully!\n";
    printTransactionReceipt("TRANSFER DEBIT", sender.getAccountNumber(), sender.getHolderName(), transferAmount, sender.getBalance());
    cout << "  (Credited to Account #" << receiver.getAccountNumber() << " - " << receiver.getHolderName() << ")\n";
}

/**
 * Displays tabular list of all registered accounts.
 */
void displayAllAccountsService() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\n>> No records found. Open an account first!\n";
        return;
    }

    BankAccount acc;
    int count = 0;

    cout << "\n=================================================================";
    cout << "\n                    MASTER ACCOUNT DIRECTORY                     ";
    cout << "\n=================================================================\n";
    cout << left << setw(10) << "Acc No."
         << setw(24) << "Holder Name"
         << setw(12) << "Type"
         << setw(12) << "Balance" << endl;
    cout << "-----------------------------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        acc.showTabularRecord();
        count++;
    }
    cout << "-----------------------------------------------------------------\n";
    cout << " Total Accounts Registered: " << count << "\n";
    cout << "=================================================================\n";
    inFile.close();
}

/**
 * Closes an account after PIN verification and reorganizes file storage.
 */
void closeAccountService() {
    int accNum;
    cout << "\n--------------------------------------------------------";
    cout << "\n                 CLOSE / DELETE ACCOUNT                 ";
    cout << "\n--------------------------------------------------------";
    cout << "\nEnter Account Number to Close: ";
    cin >> accNum;

    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << ">> No accounts found on record.\n";
        return;
    }

    BankAccount targetAcc;
    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&targetAcc), sizeof(BankAccount))) {
        if (targetAcc.getAccountNumber() == accNum) {
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << ">> [ERROR] Account #" << accNum << " not found!\n";
        return;
    }

    cout << "Account Holder: " << targetAcc.getHolderName() << endl;
    cout << "Enter 4-Digit Security PIN to confirm deletion: ";
    string enteredPin = inputMaskedPin();
    if (!targetAcc.verifyPin(enteredPin)) {
        cout << ">> [ACCESS DENIED] Incorrect PIN! Deletion aborted.\n";
        return;
    }

    cout << "Are you absolutely sure you want to close this account? (Y/N): ";
    char confirm;
    cin >> confirm;
    if (toupper(confirm) != 'Y') {
        cout << ">> Account deletion cancelled.\n";
        return;
    }

    // Reorganize file without deleted record
    inFile.open(FILE_NAME, ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    BankAccount acc;

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() != accNum) {
            outFile.write(reinterpret_cast<const char *>(&acc), sizeof(BankAccount));
        }
    }

    inFile.close();
    outFile.close();

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    cout << "\n>> [SUCCESS] Account #" << accNum << " closed and deleted permanently. <<\n";
}

// ============================================================================
// MAIN APPLICATION MENU
// ============================================================================

int main() {
    int choice;

    do {
        cout << "\n========================================================";
        cout << "\n             SECURE BANK MANAGEMENT SYSTEM              ";
        cout << "\n========================================================";
        cout << "\n  [1] Open New Account";
        cout << "\n  [2] Cash Deposit";
        cout << "\n  [3] Cash Withdrawal (PIN Protected)";
        cout << "\n  [4] Balance Inquiry & Interest Preview";
        cout << "\n  [5] Fund Transfer Between Accounts";
        cout << "\n  [6] View Master Account Directory";
        cout << "\n  [7] Close / Delete Account (PIN Protected)";
        cout << "\n  [8] Exit System";
        cout << "\n--------------------------------------------------------";
        cout << "\n  Enter your selection (1-8): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n>> Invalid input! Please enter a number between 1 and 8.\n";
            continue;
        }

        switch (choice) {
            case 1:
                createAccountService();
                break;
            case 2:
                depositService();
                break;
            case 3:
                withdrawalService();
                break;
            case 4:
                balanceInquiryService();
                break;
            case 5:
                fundTransferService();
                break;
            case 6:
                displayAllAccountsService();
                break;
            case 7:
                closeAccountService();
                break;
            case 8:
                cout << "\n========================================================";
                cout << "\n   Thank you for banking with us. Have a great day!     ";
                cout << "\n========================================================\n\n";
                break;
            default:
                cout << "\n>> Invalid choice! Please select an option from 1 to 8.\n";
        }
    } while (choice != 8);

    return 0;
}
