#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Staff {
    std::string name_;
    int id_;
    double basic_;
protected:
    Staff(std::string name, int id, double basic)
        : name_(std::move(name)), id_(id), basic_(basic) {}
    double basic() const { return basic_; }
public:
    virtual ~Staff() = default;
    const std::string& name() const { return name_; }
    int id() const { return id_; }
    virtual double pay() const = 0;
    virtual void print() const {
        std::cout << name_ << " (" << id_ << ") pay=" << pay() << '\n';
    }
};
class Professor : public Staff {
    double research_;
public:
    Professor(std::string n, int id, double b, double research)
        : Staff(std::move(n), id, b), research_(research) {}
    double pay() const override { return basic() * 1.42 + research_; }
};
class LabAssistant : public Staff {
public:
    LabAssistant(std::string n, int id, double b) : Staff(std::move(n), id, b) {}
    double pay() const override { return basic() * 1.42; }
};
class GuestFaculty : public Staff {
    double hours_, rate_;
public:
    GuestFaculty(std::string n, int id, double hours, double rate)
        : Staff(std::move(n), id, 0), hours_(hours), rate_(rate) {}
    double pay() const override { return hours_ * rate_; }
};
int main() {
    std::vector<std::unique_ptr<Staff>> staff;
    staff.push_back(std::make_unique<Professor>("Anita", 101, 50000, 12000));
    staff.push_back(std::make_unique<LabAssistant>("Bala", 102, 30000));
    staff.push_back(std::make_unique<GuestFaculty>("Charan", 103, 40, 800));
    for (const auto& person : staff) person->print();
}
