#include <iostream>
#include <string>

class SavingsAccount {
private:
    long long accountNumber;
    double balance;
    std::string transactions[10];
    int count;

    void record(const std::string& entry) {
        if (count < 10) {
            transactions[count++] = entry;
        } else {
            // Shift left: discard the oldest transaction.
            for (int i = 1; i < 10; ++i) transactions[i - 1] = transactions[i];
            transactions[9] = entry;
        }
    }

public:
    SavingsAccount(long long number, double opening)
        : accountNumber(number), balance(opening), count(0) {}

    void deposit(double amount);
    bool withdraw(double amount);
    void miniStatement() const {
        std::cout << "Account: " << accountNumber << " Balance: " << balance << '\n';
        std::cout << "Recent transactions (" << count << "):\n";
        for (int i = 0; i < count; ++i) std::cout << transactions[i] << '\n';
    }
};

void SavingsAccount::deposit(double amount) {
    if (amount <= 0) return;
    balance += amount;
    record("Deposit: +" + std::to_string(amount));
}
bool SavingsAccount::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    record("Withdrawal: -" + std::to_string(amount));
    return true;
}

int main() {
    SavingsAccount account(192421065, 1000.0);
    account.deposit(100);
    account.withdraw(50);
    for (int i = 1; i <= 10; ++i) account.deposit(i * 10);
    account.miniStatement();
}
