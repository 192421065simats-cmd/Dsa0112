#include <iostream>
#include <string>
#include <utility>

class Address {
    std::string city_;
    int pin_;
public:
    Address(std::string city, int pin) : city_(std::move(city)), pin_(pin) {
        std::cout << "Address+ ";
    }
    ~Address() { std::cout << "Address- "; }
    std::string str() const { return city_ + "-" + std::to_string(pin_); }
};
class Course {
    std::string title_;
public:
    explicit Course(std::string title) : title_(std::move(title)) { std::cout << "Course+ "; }
    ~Course() { std::cout << "Course- "; }
};
class Person {
protected:
    std::string name_;
    int age_;
    Address address_;
public:
    Person(const std::string& n, int a, const std::string& city, int pin)
        : name_(n), age_(a), address_(city, pin) { std::cout << "Person+ "; }
    virtual ~Person() { std::cout << "Person- "; }
};
class Student : public Person {
    long long regNo_;
    Course course_;
public:
    Student(const std::string& n, int a, long long reg, const std::string& city,
            int pin, const std::string& course)
        : Person(n, a, city, pin), regNo_(reg), course_(course) { std::cout << "Student+ "; }
    ~Student() override { std::cout << "Student- "; }
    void print() const {
        std::cout << regNo_ << ' ' << name_ << " (" << age_ << ") "
                  << address_.str() << '\n';
    }
};
int main() {
    std::cout << "Student lifetime: ";
    { Student s("Kavya", 19, 192311001, "Salem", 636001, "C++"); s.print(); }
    std::cout << "\n";
}
