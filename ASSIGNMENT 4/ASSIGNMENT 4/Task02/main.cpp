#include <iostream>
#include <string>
#include <utility>

class Person {
protected:
    std::string name_;
    int id_;
    std::string phone_;
public:
    Person(std::string n, int id, std::string phone)
        : name_(std::move(n)), id_(id), phone_(std::move(phone)) {}
    virtual ~Person() = default;
    void showPerson() const { std::cout << name_ << " ID=" << id_ << " Phone=" << phone_; }
};
class Student : virtual public Person {
protected:
    std::string programme_;
    double cgpa_;
public:
    Student(std::string n, int id, std::string phone, std::string prog, double cgpa)
        : Person(std::move(n), id, std::move(phone)),
          programme_(std::move(prog)), cgpa_(cgpa) {}
};
class Staff : virtual public Person {
protected:
    std::string department_;
    double salary_;
public:
    Staff(std::string n, int id, std::string phone, std::string dept, double salary)
        : Person(std::move(n), id, std::move(phone)),
          department_(std::move(dept)), salary_(salary) {}
};
class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(std::string n, int id, std::string phone,
                      std::string prog, double cgpa, std::string dept, double salary)
        : Person(n, id, phone),
          Student(n, id, phone, prog, cgpa),
          Staff(n, id, phone, dept, salary) {}
    void print() const {
        showPerson();
        std::cout << " Programme=" << programme_ << " CGPA=" << cgpa_
                  << " Department=" << department_ << " Salary=" << salary_ << '\n';
    }
};
class ResearchScholar : public Student {
    double fellowship_;
public:
    ResearchScholar(std::string n, int id, std::string phone, std::string prog,
                    double cgpa, double fellowship)
        : Person(std::move(n), id, std::move(phone)),
          Student(n, id, phone, std::move(prog), cgpa), fellowship_(fellowship) {}
    void print() const {
        showPerson(); std::cout << " Programme=" << programme_
                                << " CGPA=" << cgpa_ << " Fellowship=" << fellowship_ << '\n';
    }
};
int main() {
    TeachingAssistant ta("Kavya", 201, "9876543210", "CSE", 8.7, "IT", 35000);
    ResearchScholar rs("Arun", 202, "9123456780", "ECE", 9.1, 18000);
    ta.print(); rs.print();
}
