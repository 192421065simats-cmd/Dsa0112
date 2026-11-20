#include <iostream>
#include <stdexcept>
#include <string>

class Book {
    std::string title;
    std::string author;
    int copies;
    static int checked(int n) {
        if (n < 0) throw std::invalid_argument("copies cannot be negative");
        return n;
    }
public:
    Book() : Book("Untitled", "Unknown", 0) {}
    explicit Book(const std::string& t) : Book(t, "Unknown", 0) {}
    Book(const std::string& t, const std::string& a, int n)
        : title(t), author(a), copies(checked(n)) {}
    void print() const {
        std::cout << "Title: " << title << ", Author: " << author
                  << ", Copies: " << copies << '\n';
    }
};
int main() {
    Book empty;
    Book donated("The C++ Primer");
    Book complete("Clean Code", "Robert C. Martin", 3);
    empty.print(); donated.print(); complete.print();
    try { Book invalid("Bad record", "Unknown", -2); invalid.print(); }
    catch (const std::invalid_argument& e) { std::cout << "Rejected: " << e.what() << '\n'; }
}
