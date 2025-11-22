#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Transaction {
    string type; 
    string category;
    double amount;
    string date;
};

class FinanceManager {
    vector<Transaction> transactions;
    double savingsGoal;
public:
    FinanceManager() { savingsGoal = 0; }
    
    void addTransaction() {
        cin.ignore();
        Transaction t;
        cout << "Enter type (income/expense): ";
        getline(cin, t.type);
        cout << "Enter category: ";
        getline(cin, t.category);
        cout << "Enter amount: ";
        while(!(cin >> t.amount) || t.amount < 0) {
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid amount. Enter again: ";
        }
        cin.ignore();
        cout << "Enter date (YYYY-MM-DD): ";
        getline(cin, t.date);
        transactions.push_back(t);
        cout << "Transaction added!\n";
    }
    
    void showSummary() {
        double totalIncome=0, totalExpense=0;
        for(auto &t: transactions) {
            if(t.type=="income") totalIncome += t.amount;
            else totalExpense += t.amount;
        }
        cout << fixed << setprecision(2);
        cout << "\n--- Summary ---\n";
        cout << "Total Income: " << totalIncome << "\n";
        cout << "Total Expenses: " << totalExpense << "\n";
        cout << "Balance: " << totalIncome - totalExpense << "\n";
    }
    
    void showByCategory() {
        vector<string> categories;
        for(auto &t: transactions) {
            if(find(categories.begin(), categories.end(), t.category)==categories.end())
                categories.push_back(t.category);
        }
        cout << "\n--- Expenses by Category ---\n";
        for(auto &c: categories) {
            double sum=0;
            for(auto &t: transactions)
                if(t.category==c && t.type=="expense") sum += t.amount;
            cout << c << ": " << sum << "\n";
        }
    }
    
    void setSavingsGoal() {
        cout << "Enter savings goal amount: ";
        while(!(cin >> savingsGoal) || savingsGoal<0) {
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input. Enter again: ";
        }
        cout << "Savings goal set to " << savingsGoal << "\n";
    }
    
    void showSavingsProgress() {
        double totalIncome=0, totalExpense=0;
        for(auto &t: transactions) {
            if(t.type=="income") totalIncome+=t.amount;
            else totalExpense+=t.amount;
        }
        double balance = totalIncome - totalExpense;
        double progress = (savingsGoal>0) ? (balance/savingsGoal) : 0;
        if(progress>1) progress=1;
        int barLength = 50;
        int filled = static_cast<int>(progress*barLength);
        cout << "\n--- Savings Progress ---\n[";
        for(int i=0;i<filled;i++) cout << "#";
        for(int i=filled;i<barLength;i++) cout << "-";
        cout << "] " << fixed << setprecision(2) << progress*100 << "%\n";
        cout << "Current balance: " << balance << " / Goal: " << savingsGoal << "\n";
    }
    
    void saveToFile(const string& filename) {
        ofstream fout(filename);
        fout << savingsGoal << "\n";
        for(auto &t: transactions)
            fout << t.type << ";" << t.category << ";" << t.amount << ";" << t.date << "\n";
        fout.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream fin(filename);
        if(!fin) return;
        transactions.clear();
        fin >> savingsGoal;
        fin.ignore();
        string line;
        while(getline(fin,line)) {
            Transaction t;
            size_t pos1=line.find(';');
            size_t pos2=line.find(';',pos1+1);
            size_t pos3=line.find(';',pos2+1);
            t.type = line.substr(0,pos1);
            t.category = line.substr(pos1+1,pos2-pos1-1);
            t.amount = stod(line.substr(pos2+1,pos3-pos2-1));
            t.date = line.substr(pos3+1);
            transactions.push_back(t);
        }
        fin.close();
    }
};

int main() {
    FinanceManager fm;
    fm.loadFromFile("data.txt");
    int choice;
    do {
        cout << "\n=== Personal Finance Manager ===\n";
        cout << "1. Add Transaction\n2. Show Summary\n3. Show by Category\n";
        cout << "4. Set Savings Goal\n5. Show Savings Progress\n6. Exit\nChoice: ";
        while(!(cin>>choice)){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input. Enter number: ";
        }
        switch(choice) {
            case 1: fm.addTransaction(); break;
            case 2: fm.showSummary(); break;
            case 3: fm.showByCategory(); break;
            case 4: fm.setSavingsGoal(); break;
            case 5: fm.showSavingsProgress(); break;
            case 6: fm.saveToFile("data.txt"); cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice !=6);
    return 0;
}
