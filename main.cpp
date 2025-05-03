#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <limits>
using namespace std;

class Customer {
    public:
        string name, userid, nid, phoneNumber, email;
        int birthYear;
        double balance;
    
        Customer() {}                                               // Default constructor
    
        Customer(string n, string uid, string id, int by, string p, string e, double b) {              // Parameterized constructor 
            name = n;
            userid = uid;
            nid = id;
            birthYear = by;
            phoneNumber = p;
            email = e;
            balance = b;
        }
    
        void saveToFile() {                                             // file a customer details save korar jonno
            ofstream outFile("customers.txt", ios::app); 
            outFile << userid << "|" << name << "|" << nid << "|" << birthYear << "|"
                    << phoneNumber << "|" << email << "|" << fixed << setprecision(2) << balance << endl;
            outFile.close();
        }
    
        void savePassword(string pass) {                                            // file a password save korar jonno
            ofstream outFile("users.txt", ios::app);
            outFile << userid << "|" << pass << endl;
            outFile.close();
        }
    };
    

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class Transaction {                                                             // transaction class ,  transaction details save korar jonno
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

    void initializeManagerFile() {                                    //manager ar login , password and loan code default vabe save korar jonno
        ifstream testFile("manager.txt");
        if (!testFile.good()) {
            ofstream outFile("manager.txt");
            outFile << "admin|admin123|123456" << endl; 
            outFile.close();
        } else {
            testFile.close();
        }
    }

    void recordTransaction(string userid, string type, double amount) {                  // transaction file a transaction details save korar jonno
        ofstream outFile("transactions.txt", ios::app);
        Transaction t(type, amount);
        outFile << userid << "|" << t.type << "|" << fixed << setprecision(2) << t.amount
                << "|" << t.datetime << endl;
        outFile.close();

      
        vector<string> transactions;                                     // transaction file a save kora details gulo read korar jonno
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

    void displayHeader(string title) {
        cout << "\n========================================\n";
        cout << "              " << title << "\n";
        cout << "========================================\n";
    }

    void displayMenu(vector<string> options) {
        for (size_t i = 0; i < options.size(); i++) {
            cout << i+1 << ". " << options[i] << "\n";
        }
    }

    void returnMenuOrExit() {
        cout << "\n----------------------------------------\n";
        cout << "1. Return to Main Menu\n2. Exit\n";
        cout << "Enter your choice: ";

        int option;
        while (!(cin >> option) || (option != 1 && option != 2)) {
            cout << "Invalid input. Please enter 1 or 2: ";
            clearInputBuffer();
        }

        if (option == 2) {
            cout << "\nThank you for banking with us.\n";
            exit(0);
        }
    }

    string getValidUserId() {
        string userid;
        while (true) {
            cout << "Enter 4-digit user ID: ";
            cin >> userid;

            if (userid.length() != 4 || !all_of(userid.begin(), userid.end(), ::isdigit)) {
                cout << "Invalid input. User ID must be exactly 4 digits.\n";
                clearInputBuffer();
            } else {
                break;
            }
        }
        return userid;
    }

    void createAccount() {                                                           // account create korar jonno
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
            cout << "Invalid choice. Please enter 1-3: ";
            clearInputBuffer();
        }

        if (choice == 3) {
            return;
        }

        int total = (choice == 1) ? 1 : 0;
        if (choice == 2) {
            cout << "Number of accounts to create (1-10): ";                                          // multiple account create korar jonno
            while (!(cin >> total) || total < 1 || total > 10) {
                cout << "Invalid input. Please enter 1-10: ";
                clearInputBuffer();
            }
        }

        for (int i = 0; i < total; i++) {
            displayHeader("CREATING ACCOUNT " + to_string(i+1));

            string name, userid, nid, phoneNumber, email, password;
            int birthYear;
            double balance;

            cout << "Enter your full name: ";
            clearInputBuffer();
            getline(cin, name);

            
            while (true) {
                userid = getValidUserId();
                if (isUserIdUnique(userid)) break;
                cout << "User ID already exists. Please try another.\n";
            }

            cout << "Enter your 10-digit NID number: ";
            while (!(cin >> nid) || nid.length() != 10 || !all_of(nid.begin(), nid.end(), ::isdigit)) {
                cout << "Invalid NID. Must be 10 digits: ";
                clearInputBuffer();
            }

            cout << "Enter your birth year (1925-2025): ";
            while (!(cin >> birthYear) || birthYear < 1925 || birthYear > 2025) {
                cout << "Invalid year. Enter between 1900-2025: ";
                clearInputBuffer();
            }

            int age = 2025 - birthYear;
            if (age < 18) {
                cout << "You must be at least 18 years old to create an account.\n";
                continue;
            }

            cout << "Enter your phone number: ";
            while (!(cin >> phoneNumber) || phoneNumber.length() < 7) {
                cout << "Invalid phone number. Try again: ";
                clearInputBuffer();
            }

            cout << "Enter your email: ";
            cin >> email;

            cout << "Enter initial balance (minimum 500 TK): ";
            while (!(cin >> balance) || balance < 500) {
                cout << "Minimum balance is 500 TK. Enter amount: ";
                clearInputBuffer();
            }

            cout << "Set a password (at least 4 characters): ";
            while (!(cin >> password) || password.length() < 4) {
                cout << "Password too short. Minimum 4 characters: ";
                clearInputBuffer();
            }

            Customer newCustomer(name, userid, nid, birthYear, phoneNumber, email, balance);    // customer object create korar jonno
            newCustomer.saveToFile();
            newCustomer.savePassword(password);
            userids.push_back(userid);

            recordTransaction(userid, "DEPOSIT", balance);

            cout << "\n=== ACCOUNT CREATED SUCCESSFULLY ===\n";
            cout << "Name: " << name << "\n";
            cout << "User ID: " << 7638000000 + stoll(userid) << "\n";
            cout << "Initial Balance: " << fixed << setprecision(2) << balance << " TK\n";
        }

    
    }

    void showLastTransactions(string userid) {                                  // last 5 transaction dekhanor jonno
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
            cout << "No transactions found.\n";
            return;
        }

        int start = max(0, (int)transactions.size() - 5);
        cout << left << setw(12) << "Type" << setw(12) << "Amount" << "Date/Time\n";
        cout << string(40, '-') << endl;

        for (size_t i = start; i < transactions.size(); i++) {
            vector<string> parts = splitString(transactions[i], '|');
            if (parts.size() >= 4) {
                cout << left << setw(12) << parts[1]
                     << setw(12) << parts[2]
                     << parts[3] << endl;
            }
        }
        returnMenuOrExit();
    }

    void editContactInfo(string userid) {                                      // contact information edit korar jonno
        displayHeader("EDIT CONTACT INFORMATION");

        ifstream inFile("customers.txt");
        ofstream outFile("temp.txt");
        bool found = false;
        string line;

        while (getline(inFile, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 7 && parts[0] == userid) {
                found = true;
                cout << "\nCurrent Information:\n";
                cout << "Phone: " << parts[4] << "\n";
                cout << "Email: " << parts[5] << "\n";

                string newPhone, newEmail;
                cout << "\nEnter new phone number: ";
                cin >> newPhone;
                cout << "Enter new email: ";
                cin >> newEmail;

                outFile << parts[0] << "|" << parts[1] << "|" << parts[2] << "|"
                        << parts[3] << "|" << newPhone << "|" << newEmail << "|"
                        << parts[6] << endl;

                cout << "\nContact information updated successfully.\n";
            } else {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();
        remove("customers.txt");
        rename("temp.txt", "customers.txt");

        if (!found) {
            cout << "Account not found.\n";
        }
        returnMenuOrExit();
    }

    void userPanel(string userid) {                                               // user panel ar menu dekhanor jonno
        displayHeader("USER DASHBOARD");
        cout << "Logged in as: " << 7638000000 + stoll(userid) << "\n";

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
                cout << "Invalid choice. Enter 1-8: ";
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
 
    void managerPanel() {                                                        // manager panel ar menu dekhanor jonno
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
                cout << "Invalid choice. Enter 1-6: ";
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
        cout << "Enter new 6-digit Loan Activation Code: ";
        while (!(cin >> code) || code.length() != 6 || !all_of(code.begin(), code.end(), ::isdigit)) {
            cout << "Invalid code. Must be 6 digits: ";
            clearInputBuffer();
        }

        ofstream out("manager.txt");
        out << "admin|admin123|" << code << endl;
        cout << "\nLoan Activation Code updated successfully.\n";
    }

    string getLoanCode() {                                            // loan code read korar jonno 
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

    void deposit(string userid) {                                           // deposit korar jonno
        displayHeader("DEPOSIT");

        double amount;
        cout << "Enter deposit amount: ";
        while (!(cin >> amount) || amount <= 0) {
            cout << "Invalid amount. Enter positive value: ";
            clearInputBuffer();
        }

        updateCustomer(userid, amount);
        recordTransaction(userid, "DEPOSIT", amount);

        cout << "\nDeposit of " << fixed << setprecision(2) << amount
             << " TK completed successfully.\n";
        showBalance(userid);
    }

    void withdraw(string userid) {                                              // withdraw korar jonno  , dorkaar hole loan o nawar jonno
        displayHeader("WITHDRAWAL");

        double currentBal = getBalance(userid);
        if (currentBal < 0) {
            cout << "Your account is overdrawn. Cannot withdraw.\n";
            return;
        }

        cout << "Current Balance: " << fixed << setprecision(2) << currentBal << " TK\n";
        cout << "Enter withdrawal amount: ";

        double amount;
        while (!(cin >> amount) || amount <= 0) {
            cout << "Invalid amount. Enter positive value: ";
            clearInputBuffer();
        }

        if (currentBal - amount < 0) {
            cout << "\nWarning: This withdrawal will overdraft your account.\n";
            cout << "1. Proceed with loan\n2. Cancel\nEnter choice: ";

            int option;
            while (!(cin >> option) || (option != 1 && option != 2)) {
                cout << "Invalid choice. Enter 1 or 2: ";
                clearInputBuffer();
            }

            if (option == 2) {
                cout << "Withdrawal cancelled.\n";
                return;
            }

            string inputCode;
            cout << "Enter Loan Activation Code: ";
            cin >> inputCode;
            string managerCode = getLoanCode();

            if (inputCode != managerCode) {
                cout << "Invalid code. Withdrawal cancelled.\n";
                return;
            }

            int years;
            cout << "Repayment period (1-3 years): ";
            while (!(cin >> years) || years < 1 || years > 3) {
                cout << "Invalid duration. Enter 1-3: ";
                clearInputBuffer();
            }

            double loanAmount = abs(currentBal - amount);
            processLoan(userid, loanAmount, years);
        }

        updateCustomer(userid, -amount);
        recordTransaction(userid, "WITHDRAW", amount);

        cout << "\nWithdrawal of " << fixed << setprecision(2) << amount
             << " TK completed successfully.\n";
        showBalance(userid);
    }

    double getBalance(string userid) {
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

    void processLoan(string userid, double amount, int years) {                         // loan process korar jonno
        int months = years * 12;
        double interest = amount * 0.07;
        double totalToPay = amount + interest;
        double monthlyPayment = totalToPay / months;

        updateCustomer(userid, amount);
        recordTransaction(userid, "LOAN", amount);

        displayHeader("LOAN APPROVED");
        cout << fixed << setprecision(2);
        cout << "Loan Amount:      " << setw(10) << amount << " TK\n";
        cout << "Interest (7%):    " << setw(10) << interest << " TK\n";
        cout << "Total Repayment:  " << setw(10) << totalToPay << " TK\n";
        cout << "Monthly Payment:  " << setw(10) << monthlyPayment << " TK for " << months << " months\n";

    }

    void loan(string userid) {                                 // loan nite hole
        displayHeader("LOAN APPLICATION");

        string inputCode;
        cout << "Enter Loan Activation Code: ";
        cin >> inputCode;
        string managerCode = getLoanCode();

        if (inputCode != managerCode) {
            cout << "Invalid code. Loan application rejected.\n";
            return;
        }

        double amount;
        cout << "Enter loan amount: ";
        while (!(cin >> amount) || amount <= 0) {
            cout << "Invalid amount. Enter positive value: ";
            clearInputBuffer();
        }

        int years;
        cout << "Repayment period (1-3 years): ";
        while (!(cin >> years) || years < 1 || years > 3) {
            cout << "Invalid duration. Enter 1-3: ";
            clearInputBuffer();
        }

        processLoan(userid, amount, years);
        
    }

    void showBalance(string userid) {                                                            // balance dekhanor jonno
        double balance = getBalance(userid);
        cout << "\nCurrent Balance: " << fixed << setprecision(2) << balance << " TK\n";
        returnMenuOrExit();
    }

    bool deleteAccount(string userid) {                                        // account delete korar jonno
        displayHeader("ACCOUNT DELETION");

        ifstream in("customers.txt");
        ofstream out("temp.txt");
        bool found = false;
        string line;

        while (getline(in, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 1 && parts[0] == userid) {
                found = true;
                cout << "Account for user " << 7638000000 + stoll(userid) << " will be deleted.\n";
                cout << "Confirm deletion (Y/N): ";

                char confirm;
                cin >> confirm;
                if (toupper(confirm) != 'Y') {
                    out << line << endl;
                    cout << "Deletion cancelled.\n";
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

            cout << "Account deleted successfully.\n";
            return true;
        } else if (!found) {
            cout << "Account not found.\n";
        }
        return false;
    }

    void deleteAnyAccount() {
        displayHeader("DELETE ACCOUNT");
        string userid = getValidUserId();
        deleteAccount(userid);
    }

    void searchAccount() {
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
                cout << "User ID:      " << 7638000000 + stoll(parts[0]) << "\n";
                cout << "Name:         " << parts[1] << "\n";
                cout << "Birth Year:   " << parts[3] << "\n";
                cout << "Phone:        " << parts[4] << "\n";
                cout << "Email:        " << parts[5] << "\n";
                cout << "Balance:      " << fixed << setprecision(2) << parts[6] << " TK\n";
                break;
            }
        }

        if (!found) {
            cout << "Account not found.\n";
        }
        returnMenuOrExit();
    }

    void balanceQuery() {                                                         // balance query korar jonno
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
                cout << "User ID:      " << 7638000000 + stoll(parts[0]) << "\n";
                cout << "Name:         " << parts[1] << "\n";
                cout << "Balance:      " << fixed << setprecision(2) << parts[6] << " TK\n";
                break;
            }
        }

        if (!found) {
            cout << "Account not found.\n";
        }
        returnMenuOrExit();
    }

    void updateCustomer(string userid, double change) {
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

    void aboutUs() {                                                              // about us page dekhanor jonno
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

        cout << "Welcome to Bangladesh Bank Management System - SHIELD\n";
        cout << "----------------------------------------\n";
        cout << "Total Customers: " << customerCount << "\n";
        cout << "Total Assets:    " << fixed << setprecision(2) << totalBalance << " TK\n";
        cout << "\nDeveloped by Team NuroFive\n";
        

        returnMenuOrExit();
    }

    bool recoverPassword(string userid) {                                   // password recover korar jonno 
        displayHeader("PASSWORD RECOVERY");

        ifstream file("customers.txt");
        string line;
        while (getline(file, line)) {
            vector<string> parts = splitString(line, '|');
            if (parts.size() >= 6 && parts[0] == userid) {
                string inputPhone;
                cout << "Enter registered phone number: ";
                cin >> inputPhone;

                if (inputPhone == parts[4]) {string newPass, confirmPass;  cout << "Enter new password (min 4 chars): "; cin >> newPass; cout << "Confirm new password: "; cin >> confirmPass;

                    if (newPass == confirmPass && newPass.length() >= 4) {ifstream pfile("users.txt");  ofstream ptemp("ptemp.txt");
                        bool updated = false;

                        while (getline(pfile, line)) {vector<string> userParts = splitString(line, '|');
                            if (userParts.size() >= 1 && userParts[0] == userid) { ptemp << userid << "|" << newPass << endl; updated = true;  } 
                            else { ptemp << line << endl; }
                        }

                        pfile.close();
                        ptemp.close();
                        remove("users.txt");
                        rename("ptemp.txt", "users.txt");

                        if (updated) {cout << "\nPassword updated successfully.\n";return true; }
                    } 
                    else {cout << "Passwords don't match or are too short.\n"; return false; }
                } else {
                    cout << "Phone number doesn't match our records.\n";
                    return false;
                }
            }
        }

        cout << "User ID not found.\n";
        return false;
    }
};

int main() {
    Bank bank;
    int choice;

    do {cout << "\n     ========================================\n";
       cout << "               " << "BANGLADESH BANK" << "\n";
       cout << "     ========================================\n\n";

        cout << "Welcome to Bangladesh Bank Management system - SHIELD\n\n";
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
            cout << "Invalid choice. Enter 1-5: ";
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
                cout << "Enter password: ";
                cin >> pass;

                if (bank.validateUserLogin(userid, pass)) {
                    bank.userPanel(userid);
                } else {
                    cout << "\nLogin failed.\n";
                    cout << "1. Try again\n2. Recover password\n3. Main menu\nEnter choice: ";

                    int option;
                    while (!(cin >> option) || option < 1 || option > 3) {
                        cout << "Invalid choice. Enter 1-3: ";
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
                cout << "Enter manager ID: ";
                cin >> id;
                cout << "Enter password: ";
                cin >> pass;

                if (bank.validateManager(id, pass)) {
                    bank.managerPanel();
                } else {
                    cout << "Invalid credentials.\n";
                }
                break;
            }

            case 4:
                bank.aboutUs();
                break;

            case 5:
                cout << "\nThank you for banking with us.\n";
                exit(0);
        }
    } while (true);

    return 0;
}