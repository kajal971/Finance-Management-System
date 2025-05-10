#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

class Transaction {
protected:
    double amount;
    string description;

public:
    Transaction(double amt, const string& des) : amount(amt), description(des) {}
    virtual ~Transaction() {}

    virtual void display() const {
        cout << setw(15) << fixed << setprecision(2) << amount << setw(25) << description << endl;
    }

    virtual string getType() const = 0;

    double getAmount() const { return amount; }
    string getDescription() const { return description; }
};

class Income : public Transaction {
public:
    Income(double amt, const string& des) : Transaction(amt, des) {}

    void display() const override {
        cout << setw(15) << "Income";
        Transaction::display();
    }

    string getType() const override {
        return "Income";
    }
};

class Expenditure : public Transaction {
public:
    Expenditure(double amt, const string& des) : Transaction(amt, des) {}

    void display() const override {
        cout << setw(15) << "Expenditure";
        Transaction::display();
    }

    string getType() const override {
        return "Expenditure";
    }
};

class Investment {
protected:
    double amount;
    int duration;

public:
    Investment(double amt, int dur) : amount(amt), duration(dur) {}
    virtual ~Investment() {}

    virtual void display() const = 0;
    virtual double maturityAmount() const = 0;
    virtual string getType() const = 0;

    double getAmount() const { return amount; }
    int getDuration() const { return duration; }
};

class SIP : public Investment {
    double monthly;
public:
    SIP(double amt, int dur, double monAmt) : Investment(amt, dur), monthly(monAmt) {}

    void display() const override {
        cout << setw(15) << "SIP" << setw(15) << fixed << setprecision(2) << amount
             << setw(15) << duration << setw(20) << monthly << endl;
    }

    double maturityAmount() const override {
        double final = amount * pow(1 + (0.096 / 12), duration * 12);
        return final + (monthly * 12 * duration);
    }

    string getType() const override {
        return "SIP";
    }

    double getMonthly() const { return monthly; }
};

class FD : public Investment {
public:
    FD(double amt, int dur) : Investment(amt, dur) {}

    void display() const override {
        cout << setw(15) << "FD" << setw(15) << fixed << setprecision(2) << amount
             << setw(15) << duration << setw(20) << "-" << endl;
    }

    double maturityAmount() const override {
        return amount * pow((1 + 0.071), duration);
    }

    string getType() const override {
        return "FD";
    }
};

class Account {
public:
    string name;
    double balance;
    vector<Transaction*> transactions;
    vector<Investment*> investments;

    Account(const string& n, double b = 2000) : name(n), balance(b) {}

    void addTransaction(Transaction* t) {
        transactions.push_back(t);
        if (t->getType() == "Income") balance += t->getAmount();
        else if (t->getType() == "Expenditure") balance -= t->getAmount();
    }

    void addInvestment(Investment* inv) {
        investments.push_back(inv);
        balance -= inv->getAmount();
    }

    void displayDetails() const {
        cout << "\nAccount Name: " << name << "\nBalance: " << fixed << setprecision(2) << balance << endl;

        cout << "--Transactions--\n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(25) << "Description" << endl;
        for (auto t : transactions) t->display();

        cout << "\n--Investments--\n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(15) << "Duration" << setw(20) << "Monthly Amt" << endl;
        for (auto i : investments) {
            i->display();
            cout << " Maturity Amount: " << fixed << setprecision(2) << i->maturityAmount() << endl;
        }
    }
};

class FinanceApp {
    vector<Account> accounts;
    int currentIndex = -1;

public:
    void run() {
        loadData();
        if (accounts.empty()) addAccount(); // Prompt if no accounts

        while (true) {
            showMenu();
            int choice;
            cin >> choice;
            cin.ignore();
            if (choice == 0) break;

            switch (choice) {
                case 1: addAccount(); break;
                case 2: switchAccount(); break;
                case 3: recordTransaction(true); break;
                case 4: recordTransaction(false); break;
                case 5: addInvestment(); break;
                case 6: if (valid()) accounts[currentIndex].displayDetails(); break;
                default: cout << "Invalid choice!\n"; break;
            }
        }

        saveData();
    }

private:
    bool valid() {
        if (currentIndex < 0 || currentIndex >= accounts.size()) {
            cout << "No active account.\n";
            return false;
        }
        return true;
    }

    void showMenu() {
        cout << "\n--OPTIONS--\n";
        cout << "1. Add Account\n2. Switch Account\n3. Record INCOME\n4. Record EXPENDITURE\n";
        cout << "5. Make Investment\n6. View Account Details\n0. Exit\nEnter choice: ";
    }

    void addAccount() {
        string name;
        cout << "Enter new account name: ";
        getline(cin >> ws, name);
        accounts.emplace_back(name);
        currentIndex = accounts.size() - 1;
        cout << "Account added and switched to: " << name << endl;
    }

    void switchAccount() {
        cout << "Available accounts:\n";
        for (int i = 0; i < accounts.size(); ++i)
            cout << i << ": " << accounts[i].name << endl;

        cout << "Enter account index to switch (starting from 0): ";
        int idx;
        cin >> idx;
        if (idx >= 0 && idx < accounts.size()) {
            currentIndex = idx;
            cout << "Switched to account: " << accounts[currentIndex].name << endl;
        } else {
            cout << "Invalid index.\n";
        }
    }

    void recordTransaction(bool isIncome) {
        if (!valid()) return;
        double amt;
        string desc;
        cout << "Enter amount: ";
        cin >> amt;
        cin.ignore();
        cout << "Enter description: ";
        getline(cin, desc);
        if (!isIncome && accounts[currentIndex].balance - amt < 1000) {
            cout << "Insufficient balance.\n";
            return;
        }

        Transaction* t = isIncome ? (Transaction*)new Income(amt, desc) : (Transaction*)new Expenditure(amt, desc);
        accounts[currentIndex].addTransaction(t);
    }

    void addInvestment() {
        if (!valid()) return;
        int choice;
        cout << "1. SIP\n2. FD\nEnter type: ";
        cin >> choice;

        double amt;
        int dur;
        cout << "Enter amount: ";
        cin >> amt;
        if (accounts[currentIndex].balance - amt < 1000) {
            cout << "Not enough balance (min 1000 should remain).\n";
            return;
        }

        cout << "Enter duration in years: ";
        cin >> dur;

        if (choice == 1) {
            double monthly;
            cout << "Enter monthly amount: ";
            cin >> monthly;
            accounts[currentIndex].addInvestment(new SIP(amt, dur, monthly));
        } else if (choice == 2) {
            accounts[currentIndex].addInvestment(new FD(amt, dur));
        } else {
            cout << "Invalid investment type.\n";
        }
    }

    void saveData() {
        ofstream out("finance_data.txt");
        for (const auto& acc : accounts) {
            out << "#Account: " << acc.name << " " << acc.balance << endl;
            for (auto t : acc.transactions) {
                out << "T " << t->getType() << " " << t->getAmount() << " " << t->getDescription() << endl;
            }
            for (auto i : acc.investments) {
                out << "I " << i->getType() << " " << i->getAmount() << " " << i->getDuration();
                if (i->getType() == "SIP") {
                    out << " " << ((SIP*)i)->getMonthly();
                }
                out << endl;
            }
        }
        out.close();
    }

    void loadData() {
        ifstream in("finance_data.txt");
        if (!in) return;

        string line;
        Account* curr = nullptr;

        while (getline(in, line)) {
            if (line.rfind("#Account:", 0) == 0) {
                string name;
                double bal;
                size_t pos = line.find(":");
                name = line.substr(pos + 2, line.find_last_of(' ') - (pos + 2));
                bal = stod(line.substr(line.find_last_of(' ') + 1));
                accounts.emplace_back(name, bal);
                curr = &accounts.back();
            } else if (!curr) continue;
            else if (line[0] == 'T') {
                string type, desc;
                double amt;
                istringstream ss(line);
                string temp;
                ss >> temp >> type >> amt;
                getline(ss, desc);
                if (desc[0] == ' ') desc = desc.substr(1);
                Transaction* t = type == "Income"
    ? static_cast<Transaction*>(new Income(amt, desc))
    : static_cast<Transaction*>(new Expenditure(amt, desc));

                curr->transactions.push_back(t);
            } else if (line[0] == 'I') {
                string type;
                double amt, monthly = 0;
                int dur;
                istringstream ss(line);
                string temp;
                ss >> temp >> type >> amt >> dur;
                if (type == "SIP") ss >> monthly;

                Investment* inv = (type == "SIP") ? (Investment*)new SIP(amt, dur, monthly)
                                                 : (Investment*)new FD(amt, dur);
                curr->investments.push_back(inv);
            }
        }
        in.close();
    }
};

int main() {
    cout << "---Welcome to Finance Management System!!---\n";
    FinanceApp app;
    app.run();
    return 0;
}








