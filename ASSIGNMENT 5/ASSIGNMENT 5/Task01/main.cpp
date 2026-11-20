#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

class Bill {
protected:
    std::string patient;
    explicit Bill(const std::string& p) : patient(p) {}
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const { return "Bill"; }
    void print() const { std::cout << type() << ' ' << patient << ": " << total() << '\n'; }
};
class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(const std::string& p, double c, double t) : Bill(p), consultation(c), tests(t) {
        if (c < 0 || t < 0) throw std::invalid_argument("negative charge");
    }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};
class InPatient : public Bill {
    int days; double perDay, surgery;
public:
    InPatient(const std::string& p, int d, double rate, double s) : Bill(p), days(d), perDay(rate), surgery(s) {
        if (d < 0 || rate < 0 || s < 0) throw std::invalid_argument("negative charge");
    }
    double total() const override { return days * perDay + surgery; }
    std::string type() const override { return "IP"; }
};
class DayCare : public Bill {
    double package;
public:
    DayCare(const std::string& p, double amount) : Bill(p), package(amount) {
        if (amount < 0) throw std::invalid_argument("negative charge");
    }
    double total() const override { return package; }
    std::string type() const override { return "DayCare"; }
};
class Emergency : public Bill {
    double base, hourly; int hours;
public:
    Emergency(const std::string& p, double b, double h, int hrs) : Bill(p), base(b), hourly(h), hours(hrs) {
        if (b < 0 || h < 0 || hrs < 0) throw std::invalid_argument("negative charge");
    }
    double total() const override { return base + hourly * hours; }
    std::string type() const override { return "Emergency"; }
};
int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.emplace_back(std::make_unique<OutPatient>("Ravi", 500, 1200));
    bills.emplace_back(std::make_unique<InPatient>("Meena", 3, 2500, 40000));
    bills.emplace_back(std::make_unique<DayCare>("Arun", 18000));
    bills.emplace_back(std::make_unique<Emergency>("Latha", 2000, 500, 4));
    double sum = 0;
    for (const auto& b : bills) { b->print(); sum += b->total(); }
    std::cout << "Total " << sum << '\n';
}
