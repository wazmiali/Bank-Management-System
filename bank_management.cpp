#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
using namespace std;

class BankDB {
private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

public:
    BankDB() {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "your_password");
        con->setSchema("bank_system");
    }

    void createAccount(string name, string phone, double balance) {
        pstmt = con->prepareStatement("INSERT INTO accounts(name, phone, balance) VALUES (?, ?, ?)");
        pstmt->setString(1, name);
        pstmt->setString(2, phone);
        pstmt->setDouble(3, balance);
        pstmt->execute();
        cout << "✅ Account created successfully!\n";
        delete pstmt;
    }

    void showAccounts() {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM accounts");
        cout << "\n--- Account List ---\n";
        while (res->next()) {
            cout << "Account No: " << res->getInt("acc_no")
                 << " | Name: " << res->getString("name")
                 << " | Phone: " << res->getString("phone")
                 << " | Balance: " << res->getDouble("balance") << endl;
        }
        delete res;
        delete stmt;
    }

    void updatePhone(int acc_no, string newPhone) {
        pstmt = con->prepareStatement("UPDATE accounts SET phone=? WHERE acc_no=?");
        pstmt->setString(1, newPhone);
        pstmt->setInt(2, acc_no);
        pstmt->executeUpdate();
        cout << "📱 Phone number updated successfully!\n";
        delete pstmt;
    }

    void deposit(int acc_no, double amount) {
        pstmt = con->prepareStatement("UPDATE accounts SET balance = balance + ? WHERE acc_no=?");
        pstmt->setDouble(1, amount);
        pstmt->setInt(2, acc_no);
        pstmt->executeUpdate();
        cout << "💰 Deposit successful!\n";
        delete pstmt;
    }

    void withdraw(int acc_no, double amount) {
        pstmt = con->prepareStatement("UPDATE accounts SET balance = balance - ? WHERE acc_no=? AND balance >= ?");
        pstmt->setDouble(1, amount);
        pstmt->setInt(2, acc_no);
        pstmt->setDouble(3, amount);
        int rows = pstmt->executeUpdate();
        if (rows > 0)
            cout << "💸 Withdrawal successful!\n";
        else
            cout << "⚠️ Insufficient balance or invalid account.\n";
        delete pstmt;
    }
};

int main() {
    BankDB bank;
    int choice;
    while (true) {
        cout << "\n--- BANK MENU ---\n";
        cout << "1. Create Account\n2. Show Accounts\n3. Deposit\n4. Withdraw\n5. Update Phone\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, phone;
            double balance;
            cout << "Enter name: "; cin >> name;
            cout << "Enter phone: "; cin >> phone;
            cout << "Enter opening balance: "; cin >> balance;
            bank.createAccount(name, phone, balance);
        }
        else if (choice == 2) bank.showAccounts();
        else if (choice == 3) {
            int acc; double amt;
            cout << "Enter account no: "; cin >> acc;
            cout << "Enter amount: "; cin >> amt;
            bank.deposit(acc, amt);
        }
        else if (choice == 4) {
            int acc; double amt;
            cout << "Enter account no: "; cin >> acc;
            cout << "Enter amount: "; cin >> amt;
            bank.withdraw(acc, amt);
        }
        else if (choice == 5) {
            int acc; string phone;
            cout << "Enter account no: "; cin >> acc;
            cout << "Enter new phone: "; cin >> phone;
            bank.updatePhone(acc, phone);
        }
        else if (choice == 6) break;
        else cout << "Invalid choice!\n";
    }
    return 0;
}
