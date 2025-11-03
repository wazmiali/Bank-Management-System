/*
Bank Management System with Update Feature (C++)
Compile: g++ bank_management.cpp -o bank_management
Run: ./bank_management

Features:
 - Create account (with phone number)
 - View, Deposit, Withdraw, Transfer
 - List all accounts
 - Delete account
 - Update account phone number
 - File-based persistence (bank_data.txt)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
using namespace std;

class Account {
public:
    long accountNumber;
    string holderName;
    string phoneNumber;
    double balance;

    Account() {}
    Account(long accNo, string name, string phone, double bal) {
        accountNumber = accNo;
        holderName = name;
        phoneNumber = phone;
        balance = bal;
    }

    void deposit(double amt) {
        if (amt <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }
        balance += amt;
    }

    bool withdraw(double amt) {
        if (amt <= 0 || amt > balance) {
            cout << "Invalid or insufficient balance.\n";
            return false;
        }
        balance -= amt;
        return true;
    }

    void display() {
        cout << "\nAccount No: " << accountNumber
             << "\nHolder: " << holderName
             << "\nPhone: " << phoneNumber
             << "\nBalance: " << fixed << setprecision(2) << balance << "\n";
    }
};

class Bank {
    map<long, Account> accounts;
    long nextAccountNumber = 1000000000;

public:
    void createAccount() {
        string name, phone;
        double init;
        cout << "Enter account holder name: ";
        getline(cin, name);
        cout << "Enter phone number: ";
        getline(cin, phone);
        cout << "Enter initial deposit: ";
        cin >> init;
        cin.ignore();

        Account a(nextAccountNumber++, name, phone, init);
        accounts[a.accountNumber] = a;
        cout << "Account created. Account No: " << a.accountNumber << "\n";
    }

    void viewAccount() {
        long acc;
        cout << "Enter account number: ";
        cin >> acc;
        cin.ignore();
        if (accounts.find(acc) != accounts.end())
            accounts[acc].display();
        else
            cout << "Account not found.\n";
    }

    void deposit() {
        long acc;
        double amt;
        cout << "Enter account number: ";
        cin >> acc;
        cout << "Enter deposit amount: ";
        cin >> amt;
        cin.ignore();
        if (accounts.find(acc) != accounts.end()) {
            accounts[acc].deposit(amt);
            cout << "Deposit successful.\n";
        } else cout << "Account not found.\n";
    }

    void withdraw() {
        long acc;
        double amt;
        cout << "Enter account number: ";
        cin >> acc;
        cout << "Enter withdrawal amount: ";
        cin >> amt;
        cin.ignore();
        if (accounts.find(acc) != accounts.end()) {
            accounts[acc].withdraw(amt);
        } else cout << "Account not found.\n";
    }

    void transfer() {
        long from, to;
        double amt;
        cout << "From account: ";
        cin >> from;
        cout << "To account: ";
        cin >> to;
        cout << "Amount: ";
        cin >> amt;
        cin.ignore();

        if (accounts.find(from) == accounts.end() || accounts.find(to) == accounts.end()) {
            cout << "Invalid account(s).\n";
            return;
        }
        if (accounts[from].withdraw(amt)) {
            accounts[to].deposit(amt);
            cout << "Transfer successful.\n";
        }
    }

    void listAccounts() {
        if (accounts.empty()) {
            cout << "No accounts found.\n";
            return;
        }
        cout << left << setw(15) << "Account No" << setw(25) << "Holder Name" << setw(15) << "Phone" << setw(10) << "Balance" << "\n";
        for (auto &p : accounts)
            cout << left << setw(15) << p.second.accountNumber << setw(25) << p.second.holderName << setw(15) << p.second.phoneNumber << setw(10) << fixed << setprecision(2) << p.second.balance << "\n";
    }

    void deleteAccount() {
        long acc;
        cout << "Enter account number to delete: ";
        cin >> acc;
        cin.ignore();
        if (accounts.erase(acc))
            cout << "Account deleted.\n";
        else
            cout << "Account not found.\n";
    }

    void updatePhoneNumber() {
        long acc;
        cout << "Enter account number: ";
        cin >> acc;
        cin.ignore();
        if (accounts.find(acc) != accounts.end()) {
            string newPhone;
            cout << "Enter new phone number: ";
            getline(cin, newPhone);
            accounts[acc].phoneNumber = newPhone;
            cout << "Phone number updated successfully.\n";
        } else {
            cout << "Account not found.\n";
        }
    }

    void saveToFile() {
        ofstream out("bank_data.txt");
        for (auto &p : accounts)
            out << p.second.accountNumber << '|' << p.second.holderName << '|' << p.second.phoneNumber << '|' << p.second.balance << "\n";
        out.close();
    }

    void loadFromFile() {
        ifstream in("bank_data.txt");
        if (!in) return;
        accounts.clear();
        while (!in.eof()) {
            string line;
            if (!getline(in, line)) break;
            if (line.empty()) continue;

            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);

            long acc = stol(line.substr(0, p1));
            string name = line.substr(p1 + 1, p2 - p1 - 1);
            string phone = line.substr(p2 + 1, p3 - p2 - 1);
            double bal = stod(line.substr(p3 + 1));

            accounts[acc] = Account(acc, name, phone, bal);
            if (acc >= nextAccountNumber) nextAccountNumber = acc + 1;
        }
        in.close();
    }
};

int main() {
    Bank bank;
    bank.loadFromFile();
    int choice;

    while (true) {
        cout << "\n=== Bank Management System ===\n";
        cout << "1. Create Account\n2. View Account\n3. Deposit\n4. Withdraw\n5. Transfer\n6. List Accounts\n7. Delete Account\n8. Update Phone Number\n9. Save & Exit\nChoose: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.viewAccount(); break;
            case 3: bank.deposit(); break;
            case 4: bank.withdraw(); break;
            case 5: bank.transfer(); break;
            case 6: bank.listAccounts(); break;
            case 7: bank.deleteAccount(); break;
            case 8: bank.updatePhoneNumber(); break;
            case 9: bank.saveToFile(); cout << "Data saved. Exiting...\n"; return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}
