#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <limits>
using namespace std;


#define RESET   "\033[0m"                                                                                        // terminal output colorful dekhanor jonno
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"

class Customer {
public:
    string name, userid, nid, phoneNumber, email;
    int birthYear;
    double balance;

    Customer() {}                                                                                            // Default constructor customer class ar jonno

    Customer(string n, string uid, string id, int by, string p, string e, double b) {
        name = n;
        userid = uid;
        nid = id;
        birthYear = by;
        phoneNumber = p;
        email = e;
        balance = b;
    }

    void saveToFile() {                                                                                     // customer class ar data save korar jonno function
        ofstream outFile("customers.txt", ios::app);
        outFile << userid << "|" << name << "|" << nid << "|" << birthYear << "|"
                << phoneNumber << "|" << email << "|" << fixed << setprecision(2) << balance << endl;
        outFile.close();
    }

    void savePassword(string pass) {                                                           // customer class ar password save korar jonno function
        ofstream outFile("users.txt", ios::app);
        outFile << userid << "|" << pass << endl;
        outFile.close();
    }
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class Transaction {                                                                            // transaction class ar data save korar jonno function
public:
    string type;
    double amount;
    string datetime;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        time_t now = time(0);
        datetime = ctime(&now);
        datetime = datetime.substr(0, datetime.length()-1);
    }
};

class Bank {
protected:
    vector<string> userids;

    void loadExistingUserIds() {
        ifstream inFile("customers.txt");
        string line;
        while (getline(inFile, line)) {
            size_t pos = line.find("|");
            if (pos != string::npos) {
                userids.push_back(line.substr(0, pos));
            }
        }
        inFile.close();
    }

    void initializeManagerFile() {                                                          // manager file default password save korar jonno function
        ifstream testFile("manager.txt");
        if (!testFile.good()) {
            ofstream outFile("manager.txt");
            outFile << "admin|admin123|123456" << endl;
            outFile.close();
        } else {
            testFile.close();
        }
    }

    void recordTransaction(string userid, string type, double amount) {                       // transaction file e data save korar jonno function
        ofstream outFile("transactions.txt", ios::app);
        Transaction t(type, amount);
        outFile << userid << "|" << t.type << "|" << fixed << setprecision(2) << t.amount
                << "|" << t.datetime << endl;
        outFile.close();

        vector<string> transactions;
        ifstream inFile("transactions.txt");
        string line;
        while (getline(inFile, line)) {
            if (line.find(userid + "|") == 0) {
                transactions.push_back(line);
            }
        }
        inFile.close();

        if (transactions.size() > 5) {
            ofstream tempFile("temp_trans.txt");
            for (size_t i = transactions.size() - 5; i < transactions.size(); i++) {
                tempFile << transactions[i] << endl;
            }
            tempFile.close();
            remove("transactions.txt");
            rename("temp_trans.txt", "transactions.txt");
        }
    }

    vector<string> splitString(const string &s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    Bank() {
        initializeManagerFile();
        loadExistingUserIds();
    }

    bool isUserIdUnique(string uid) {
        return find(userids.begin(), userids.end(), uid) == userids.end();
    }

    bool validateUserLogin(string uid, string pass) {
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 2 && parts[0] == uid && parts[1] == pass) {
                return true;
            }
        }
        return false;
    }

    bool validateManager(string id, string pass) {
        ifstream file("manager.txt");
        string line;
        while (getline(file, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 2 && parts[0] == id && parts[1] == pass) {
                return true;
            }
        }
        return false;
    }

    void displayHeader(string title) {                                                       // header display korar jonno function (jate sob ak rokom dekhay)
        cout << "\n" << BOLD << CYAN;
        cout << "========================================\n";
        cout << "            " << title << "           \n";
        cout << "========================================\n" << RESET;
    }

    void displayMenu(vector<string> options) {
        for (size_t i = 0; i < options.size(); i++) {
            cout << GREEN << " " << (i+1) << ". " << RESET << options[i] << "\n";
        }
        cout << "\n";
    }

    void returnMenuOrExit() {                                                      // menu theke ber hote chaile exit korar jonno function
        cout << "\n" << YELLOW << "----------------------------------------\n" << RESET;
        cout << BOLD << "1. Return to Main Menu\n2. Exit\n" << RESET;
        cout << YELLOW << "----------------------------------------\n" << RESET;
        cout << "Enter your choice: ";

        int option;
        while (!(cin >> option) || (option != 1 && option != 2)) {
            cout << RED << "Invalid input. Please enter 1 or 2: " << RESET;
            clearInputBuffer();
        }

        if (option == 2) {
            cout << "\n" << GREEN << "========================================\n";
            cout << "   Thank you for banking with us!\n";
            cout << "========================================\n" << RESET;
            exit(0);
        }
    }
 
    string getValidUserId() {                                                        // user id input nite function
        string userid; 
        while (true) {
            cout << BOLD << "Enter 4-digit user ID: " << RESET;
            cin >> userid;

            if (userid.length() != 4 || !all_of(userid.begin(), userid.end(), ::isdigit)) {
                cout << RED << "Invalid input. User ID must be exactly 4 digits.\n" << RESET;
                clearInputBuffer();
            } else {
                break;
            }
        }
        return userid;
    }

    void createAccount() {                                                         // account create korar jonno function
        displayHeader("ACCOUNT CREATION");
        vector<string> options = {
            "Single Account",
            "Multiple Accounts",
            "Return to Main Menu"
        };
        displayMenu(options);
        cout << "Enter your choice: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << RED << "Invalid choice. Please enter 1-3: " << RESET;
            clearInputBuffer();
        }

        if (choice == 3) {
            return;
        }

        int total = (choice == 1) ? 1 : 0;
        if (choice == 2) {
            cout << "Number of accounts to create (1-10): ";
            while (!(cin >> total) || total < 1 || total > 10) {
                cout << RED << "Invalid input. Please enter 1-10: " << RESET;
                clearInputBuffer();
            }
        }

        for (int i = 0; i < total; i++) {                                           //multiple account create korar jonno loop
            displayHeader("CREATING ACCOUNT " + to_string(i+1));

            string name, userid, nid, phoneNumber, email, password;
            int birthYear;
            double balance;

            cout << BOLD << "Enter your full name: " << RESET;
            clearInputBuffer();
            getline(cin, name);

            while (true) {
                userid = getValidUserId();
                if (isUserIdUnique(userid)) break;
                cout << RED << "User ID already exists. Please try another.\n" << RESET;
            }

            cout << BOLD << "Enter your 10-digit NID number: " << RESET;
            while (!(cin >> nid) || nid.length() != 10 || !all_of(nid.begin(), nid.end(), ::isdigit)) {
                cout << RED << "Invalid NID. Must be 10 digits: " << RESET;
                clearInputBuffer();
            }

            cout << BOLD << "Enter your birth year (1925-2025): " << RESET;
            while (!(cin >> birthYear) || birthYear < 1925 || birthYear > 2025) {
                cout << RED << "Invalid year. Enter between 1900-2025: " << RESET;
                clearInputBuffer();
            }

            int age = 2025 - birthYear;
            if (age < 18) {
                cout << RED << "\nYou must be at least 18 years old to create an account.\n" << RESET;
                continue;
            }

            cout << BOLD << "Enter your phone number: " << RESET;
            while (!(cin >> phoneNumber) || phoneNumber.length() < 7) {
                cout << RED << "Invalid phone number. Try again: " << RESET;
                clearInputBuffer();
            }

            cout << BOLD << "Enter your email: " << RESET;
            cin >> email;

            cout << BOLD << "Enter initial balance (minimum 500 TK): " << RESET;
            while (!(cin >> balance) || balance < 500) {
                cout << RED << "Minimum balance is 500 TK. Enter amount: " << RESET;
                clearInputBuffer();
            }

            cout << BOLD << "Set a password (at least 4 characters): " << RESET;
            while (!(cin >> password) || password.length() < 4) {
                cout << RED << "Password too short. Minimum 4 characters: " << RESET;
                clearInputBuffer();
            }

            Customer newCustomer(name, userid, nid, birthYear, phoneNumber, email, balance);                             
            newCustomer.saveToFile();
            newCustomer.savePassword(password);
            userids.push_back(userid);

            recordTransaction(userid, "DEPOSIT", balance);

            cout << GREEN << "\n========================================\n";
            cout << "      ACCOUNT CREATED SUCCESSFULLY\n";
            cout << "========================================\n" << RESET;
            cout << BOLD << "Name: " << RESET << name << "\n";
            cout << BOLD << "User ID: " << RESET << "7638" << userid << "\n";
            cout << BOLD << "Initial Balance: " << RESET << fixed << setprecision(2) << balance << " TK\n";
        }
    }

    void showLastTransactions(string userid) {                                    // last 5 transactions dekhar jonno function
        displayHeader("LAST 5 TRANSACTIONS");

        ifstream inFile("transactions.txt");
        vector<string> transactions;
        string line;

        while (getline(inFile, line)) {
            if (line.find(userid + "|") == 0) {
                transactions.push_back(line);
            }
        }
        inFile.close();

        if (transactions.empty()) {
            cout << YELLOW << "No transactions found.\n" << RESET;
            return;
        }

        int start = max(0, (int)transactions.size() - 5);
        cout << BOLD << left << setw(12) << "Type" << setw(12) << "Amount" << "Date/Time\n" << RESET;
        cout << string(40, '-') << endl;

        for (size_t i = start; i < transactions.size(); i++) {
            vector<string> parts = splitString(transactions[i], '|');
            if (parts.size() >= 4) {
                string typeColor = (parts[1] == "DEPOSIT") ? GREEN : RED;
                cout << typeColor << left << setw(12) << parts[1] << RESET
                     << BOLD << setw(12) << parts[2] << RESET
                     << parts[3] << endl;
            }
        }
        returnMenuOrExit();
    }

    void editContactInfo(string userid) {                                                    // contact information edit korar jonno function
        displayHeader("EDIT CONTACT INFORMATION");

        ifstream inFile("customers.txt");
        ofstream outFile("temp.txt");
        bool found = false;
        string line;

        while (getline(inFile, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                found = true;
                cout << "\n" << BOLD << "Current Information:\n" << RESET;
                cout << "Phone: " << parts[4] << "\n";
                cout << "Email: " << parts[5] << "\n";

                string newPhone, newEmail;
                cout << "\n" << BOLD << "Enter new phone number: " << RESET;
                cin >> newPhone;
                cout << BOLD << "Enter new email: " << RESET;
                cin >> newEmail;

                outFile << parts[0] << "|" << parts[1] << "|" << parts[2] << "|"
                        << parts[3] << "|" << newPhone << "|" << newEmail << "|"
                        << parts[6] << endl;

                cout << GREEN << "\nContact information updated successfully.\n" << RESET;
            } else {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();
        remove("customers.txt");
        rename("temp.txt", "customers.txt");

        if (!found) {
            cout << RED << "Account not found.\n" << RESET;
        }
        returnMenuOrExit();
    }

    void userPanel(string userid) {                                                           // user panel e jate user login hoye kaj korte pare
        displayHeader("USER DASHBOARD");
        cout << BOLD << "Logged in as: " << RESET << "7638" << userid << "\n";

        int choice;
        do {
            displayHeader("USER MENU");
            vector<string> options = {
                "Deposit",
                "Withdraw",
                "Take Loan",
                "Check Balance",
                "Delete Account",
                "View Transactions",
                "Edit Contact Info",
                "Logout"
            };
            displayMenu(options);
            cout << "Enter your choice: ";

            while (!(cin >> choice) || choice < 1 || choice > 8) {
                cout << RED << "Invalid choice. Enter 1-8: " << RESET;
                clearInputBuffer();
            }

            switch (choice) {
                case 1: deposit(userid); break;
                case 2: withdraw(userid); break;
                case 3: loan(userid); break;
                case 4: showBalance(userid); break;
                case 5:
                    if (deleteAccount(userid)) return;
                    break;
                case 6: showLastTransactions(userid); break;
                case 7: editContactInfo(userid); break;
                case 8: return;
            }
        } while (true);
    }

    void managerPanel() {                                                               // manager panel ar menu dekhanor jonno function
        displayHeader("MANAGER DASHBOARD");

        int choice;
        do {
            displayHeader("MANAGER MENU");
            vector<string> options = {
                "Search Account",
                "Balance Query",
                "Delete Account",
                "Setup Loan Code",
                "View Transactions",
                "Logout"
            };
            displayMenu(options);
            cout << "Enter your choice: ";

            while (!(cin >> choice) || choice < 1 || choice > 6) {
                cout << RED << "Invalid choice. Enter 1-6: " << RESET;
                clearInputBuffer();
            }

            switch (choice) {
                case 1: searchAccount(); break;
                case 2: balanceQuery(); break;
                case 3: deleteAnyAccount(); break;
                case 4: setupLoanCode(); break;
                case 5: viewCustomerTransactions(); break;
                case 6: return;
            }
        } while (true);
    }

    void viewCustomerTransactions() {
        displayHeader("VIEW CUSTOMER TRANSACTIONS");
        string userid = getValidUserId();
        showLastTransactions(userid);
    }

    void setupLoanCode() {
        displayHeader("SETUP LOAN CODE");

        string code;
        cout << BOLD << "Enter new 6-digit Loan Activation Code: " << RESET;
        while (!(cin >> code) || code.length() != 6 || !all_of(code.begin(), code.end(), ::isdigit)) {
            cout << RED << "Invalid code. Must be 6 digits: " << RESET;
            clearInputBuffer();
        }

        ofstream out("manager.txt");
        out << "admin|admin123|" << code << endl;
        cout << GREEN << "\nLoan Activation Code updated successfully.\n" << RESET;
    }

    string getLoanCode() {                                                                      // loan activation code pabar jonno function
        ifstream file("manager.txt");
        string line;
        if (getline(file, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 3) {
                return parts[2];
            }
        }
        return "";
    }

    void deposit(string userid) {                                                             // deposit korar jonno function
        displayHeader("DEPOSIT");

        double amount;
        cout << BOLD << "Enter deposit amount: " << RESET;
        while (!(cin >> amount) || amount <= 0) {
            cout << RED << "Invalid amount. Enter positive value: " << RESET;
            clearInputBuffer();
        }

        updateCustomer(userid, amount);
        recordTransaction(userid, "DEPOSIT", amount);

        cout << GREEN << "\nDeposit of " << fixed << setprecision(2) << amount
             << " TK completed successfully.\n" << RESET;
        showBalance(userid);
    }

    void withdraw(string userid) {                                                            // withdraw korar jonno function
        displayHeader("WITHDRAWAL");

        double currentBal = getBalance(userid);
        if (currentBal < 0) {
            cout << RED << "Your account is overdrawn. Cannot withdraw.\n" << RESET;
            return;
        }

        cout << BOLD << "Current Balance: " << RESET << fixed << setprecision(2) << currentBal << " TK\n";
        cout << BOLD << "Enter withdrawal amount: " << RESET;

        double amount;
        while (!(cin >> amount) || amount <= 0) {
            cout << RED << "Invalid amount. Enter positive value: " << RESET;
            clearInputBuffer();
        }

        if (currentBal - amount < 0) {
            cout << YELLOW << "\nWarning: This withdrawal will overdraft your account.\n" << RESET;
            cout << "1. Proceed with loan\n2. Cancel\nEnter choice: ";

            int option;
            while (!(cin >> option) || (option != 1 && option != 2)) {
                cout << RED << "Invalid choice. Enter 1 or 2: " << RESET;
                clearInputBuffer();
            }

            if (option == 2) {
                cout << YELLOW << "Withdrawal cancelled.\n" << RESET;
                return;
            }

            string inputCode;
            cout << BOLD << "Enter Loan Activation Code: " << RESET;
            cin >> inputCode;
            string managerCode = getLoanCode();

            if (inputCode != managerCode) {
                cout << RED << "Invalid code. Withdrawal cancelled.\n" << RESET;
                return;
            }

            int years;
            cout << BOLD << "Repayment period (1-3 years): " << RESET;
            while (!(cin >> years) || years < 1 || years > 3) {
                cout << RED << "Invalid duration. Enter 1-3: " << RESET;
                clearInputBuffer();
            }

            double loanAmount = abs(currentBal - amount);
            processLoan(userid, loanAmount, years);
        }

        updateCustomer(userid, -amount);
        recordTransaction(userid, "WITHDRAW", amount);

        cout << GREEN << "\nWithdrawal of " << fixed << setprecision(2) << amount
             << " TK completed successfully.\n" << RESET;
        showBalance(userid);
    }

    double getBalance(string userid) {                                                                // balance pabar jonno function
        ifstream in("customers.txt");
        string line;
        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                return stod(parts[6]);
            }
        }
        return 0.0;
    }
 
    void processLoan(string userid, double amount, int years) {                                      // loan process ar por show korar jonno function
        int months = years * 12;
        double interest = amount * 0.07;
        double totalToPay = amount + interest;
        double monthlyPayment = totalToPay / months;

        updateCustomer(userid, amount);
        recordTransaction(userid, "LOAN", amount);

        displayHeader("LOAN APPROVED");
        cout << fixed << setprecision(2);
        cout << BOLD << "Loan Amount:      " << RESET << setw(10) << amount << " TK\n";
        cout << BOLD << "Interest (7%):    " << RESET << setw(10) << interest << " TK\n";
        cout << BOLD << "Total Repayment:  " << RESET << setw(10) << totalToPay << " TK\n";
        cout << BOLD << "Monthly Payment:  " << RESET << setw(10) << monthlyPayment << " TK for " << months << " months\n";
    }

    void loan(string userid) {                                                                        // loan nawar jonno function
        displayHeader("LOAN APPLICATION");

        string inputCode;
        cout << BOLD << "Enter Loan Activation Code: " << RESET;
        cin >> inputCode;
        string managerCode = getLoanCode();

        if (inputCode != managerCode) {
            cout << RED << "Invalid code. Loan application rejected.\n" << RESET;
            return;
        }

        double amount;
        cout << BOLD << "Enter loan amount: " << RESET;
        while (!(cin >> amount) || amount <= 0) {
            cout << RED << "Invalid amount. Enter positive value: " << RESET;
            clearInputBuffer();
        }

        int years;
        cout << BOLD << "Repayment period (1-3 years): " << RESET;
        while (!(cin >> years) || years < 1 || years > 3) {
            cout << RED << "Invalid duration. Enter 1-3: " << RESET;
            clearInputBuffer();
        }

        processLoan(userid, amount, years);
    }

    void showBalance(string userid) {                                                                 // balance show korar jonno function
        double balance = getBalance(userid);
        cout << "\n" << BOLD << "Current Balance: " << RESET << fixed << setprecision(2) << balance << " TK\n";
        returnMenuOrExit();
    }

    bool deleteAccount(string userid) {                                                                // account delete korar jonno function
        displayHeader("ACCOUNT DELETION");

        ifstream in("customers.txt");
        ofstream out("temp.txt");
        bool found = false;
        string line;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 1 && parts[0] == userid) {
                found = true;
                cout << "Account for user " << "7638" << userid << " will be deleted.\n";
                cout << BOLD << "Confirm deletion (Y/N): " << RESET;

                char confirm;
                cin >> confirm;
                if (toupper(confirm) != 'Y') {
                    out << line << endl;
                    cout << YELLOW << "Deletion cancelled.\n" << RESET;
                    found = false;
                }
            } else {
                out << line << endl;
            }
        }

        in.close();
        out.close();
        remove("customers.txt");
        rename("temp.txt", "customers.txt");

        if (found) {
            ifstream pfile("users.txt");
            ofstream ptemp("ptemp.txt");
            while (getline(pfile, line)) {
                vector<string> parts = splitString(line, '|');
                if (parts.empty() || parts[0] != userid) {
                    ptemp << line << endl;
                }
            }
            pfile.close();
            ptemp.close();
            remove("users.txt");
            rename("ptemp.txt", "users.txt");

            ifstream tfile("transactions.txt");
            ofstream ttemp("ttemp.txt");
            while (getline(tfile, line)) {
                if (line.find(userid + "|") != 0) {
                    ttemp << line << endl;
                }
            }
            tfile.close();
            ttemp.close();
            remove("transactions.txt");
            rename("ttemp.txt", "transactions.txt");

            userids.erase(remove(userids.begin(), userids.end(), userid), userids.end());

            cout << GREEN << "Account deleted successfully.\n" << RESET;
            return true;
        } else if (!found) {
            cout << RED << "Account not found.\n" << RESET;
        }
        return false;
    }

    void deleteAnyAccount() {                                                                            // manager jonno j kono account delete account korar jonno function
        displayHeader("DELETE ACCOUNT");
        string userid = getValidUserId();
        deleteAccount(userid);
    }

    void searchAccount() {                                                                                  // account search korar jonno function
        displayHeader("ACCOUNT SEARCH");
        string userid = getValidUserId();

        ifstream in("customers.txt");
        string line;
        bool found = false;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                found = true;
                displayHeader("ACCOUNT DETAILS");
                cout << BOLD << "User ID:      " << RESET << "7638" << parts[0] << "\n";
                cout << BOLD << "Name:         " << RESET << parts[1] << "\n";
                cout << BOLD << "Birth Year:   " << RESET << parts[3] << "\n";
                cout << BOLD << "Phone:        " << RESET << parts[4] << "\n";
                cout << BOLD << "Email:        " << RESET << parts[5] << "\n";
                cout << BOLD << "Balance:      " << RESET << fixed << setprecision(2) << parts[6] << " TK\n";
                break;
            }
        }

        if (!found) {
            cout << RED << "Account not found.\n" << RESET;
        }
        returnMenuOrExit();
    }

    void balanceQuery() {
        displayHeader("BALANCE QUERY");
        string userid = getValidUserId();

        ifstream in("customers.txt");
        string line;
        bool found = false;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                found = true;
                displayHeader("BALANCE QUERY");
                cout << BOLD << "User ID:      " << RESET << "7638" << parts[0] << "\n";
                cout << BOLD << "Name:         " << RESET << parts[1] << "\n";
                cout << BOLD << "Balance:      " << RESET << fixed << setprecision(2) << parts[6] << " TK\n";
                break;
            }
        }

        if (!found) {
            cout << RED << "Account not found.\n" << RESET;
        }
        returnMenuOrExit();
    }

    void updateCustomer(string userid, double change) {                                                        // customer balance update korar jonno function
        ifstream in("customers.txt");
        ofstream out("temp.txt");
        string line;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                double newBalance = stod(parts[6]) + change;
                out << parts[0] << "|" << parts[1] << "|" << parts[2] << "|"
                    << parts[3] << "|" << parts[4] << "|" << parts[5] << "|"
                    << fixed << setprecision(2) << newBalance << endl;
            } else {
                out << line << endl;
            }
        }

        in.close();
        out.close();
        remove("customers.txt");
        rename("temp.txt", "customers.txt");
    }

    void aboutUs() {                                                                                           // about us page dekhanor jonno function
        displayHeader("ABOUT BANGLADESH BANK");

        ifstream in("customers.txt");
        double totalBalance = 0;
        int customerCount = 0;
        string line;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7) {
                totalBalance += stod(parts[6]);
                customerCount++;
            }
        }

        cout << BOLD << CYAN << "Welcome to Bangladesh Bank Management System - SHIELD\n" << RESET;
        cout << YELLOW << "----------------------------------------\n" << RESET;
        cout << BOLD << "Total Customers: " << RESET << customerCount << "\n";
        cout << BOLD << "Total Assets:    " << RESET << fixed << setprecision(2) << totalBalance << " TK\n";
        cout << BOLD << "Contact : 01786500883 \n" << RESET;
        cout << BOLD << "Email:nurofive01@gmail.com\n" << RESET;
        cout << BOLD << "Website: www.nurofive.com\n" << RESET;
        cout << "\n" << MAGENTA << "Developed by Team NuroFive\n" << RESET;

        returnMenuOrExit();
    }

    bool recoverPassword(string userid) {                                                                     // password recover korar jonno function
        displayHeader("PASSWORD RECOVERY");

        ifstream file("customers.txt");
        string line;
        while (getline(file, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 6 && parts[0] == userid) {
                string inputPhone;
                cout << BOLD << "Enter registered phone number: " << RESET;
                cin >> inputPhone;

                if (inputPhone == parts[4]) {
                    string newPass, confirmPass;
                    cout << BOLD << "Enter new password (min 4 chars): " << RESET;
                    cin >> newPass;
                    cout << BOLD << "Confirm new password: " << RESET;
                    cin >> confirmPass;

                    if (newPass == confirmPass && newPass.length() >= 4) {
                        ifstream pfile("users.txt");
                        ofstream ptemp("ptemp.txt");
                        bool updated = false;

                        while (getline(pfile, line)) {
                            vector<string> userParts = splitString(line, '|');
                            if (userParts.size() >= 1 && userParts[0] == userid) {
                                ptemp << userid << "|" << newPass << endl;
                                updated = true;
                            } else {
                                ptemp << line << endl;
                            }
                        }

                        pfile.close();
                        ptemp.close();
                        remove("users.txt");
                        rename("ptemp.txt", "users.txt");

                        if (updated) {
                            cout << GREEN << "\nPassword updated successfully.\n" << RESET;
                            return true;
                        }
                    } else {
                        cout << RED << "Passwords don't match or are too short.\n" << RESET;
                        return false;
                    }
                } else {
                    cout << RED << "Phone number doesn't match our records.\n" << RESET;
                    return false;
                }
            }
        }

        cout << RED << "User ID not found.\n" << RESET;
        return false;
    }
};

int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n" << BOLD << BLUE;
        cout << "========================================\n";
        cout << "        BANGLADESH BANK - SHIELD\n";
        cout << "========================================\n\n" << RESET;

        cout << BOLD << "Welcome to Bangladesh Bank Management System\n\n" << RESET;
        vector<string> options = {
            "Create Account",
            "User Login",
            "Manager Login",
            "About Us",
            "Exit"
        };
        bank.displayMenu(options);
        cout << "Enter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << RED << "Invalid choice. Enter 1-5: " << RESET;
            clearInputBuffer();
        }

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2: {
                bank.displayHeader("USER LOGIN");
                string userid = bank.getValidUserId();
                string pass;
                cout << BOLD << "Enter password: " << RESET;
                cin >> pass;

                if (bank.validateUserLogin(userid, pass)) {
                    bank.userPanel(userid);
                } else {
                    cout << RED << "\nLogin failed.\n" << RESET;
                    cout << "1. Try again\n2. Recover password\n3. Main menu\nEnter choice: ";

                    int option;
                    while (!(cin >> option) || option < 1 || option > 3) {
                        cout << RED << "Invalid choice. Enter 1-3: " << RESET;
                        clearInputBuffer();
                    }

                    if (option == 2) {
                        bank.recoverPassword(userid);
                    } else if (option == 3) {
                        break;
                    }
                }
                break;
            }

            case 3: {
                bank.displayHeader("MANAGER LOGIN");
                string id, pass;
                cout << BOLD << "Enter manager ID: " << RESET;
                cin >> id;
                cout << BOLD << "Enter password: " << RESET;
                cin >> pass;

                if (bank.validateManager(id, pass)) {
                    bank.managerPanel();
                } else {
                    cout << RED << "Invalid credentials.\n" << RESET;
                }
                break;
            }

            case 4:
                bank.aboutUs();
                break;

            case 5:
                cout << "\n" << GREEN << "========================================\n";
                cout << "   Thank you for banking with us!\n";
                cout << "========================================\n" << RESET;
                exit(0);
        }
    } while (true);

    return 0;
}