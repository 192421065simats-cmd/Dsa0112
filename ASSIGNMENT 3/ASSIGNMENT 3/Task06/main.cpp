#include <cstring>
#include <iostream>
#include <stdexcept>

class Text {
    char* data;
public:
    explicit Text(const char* s = "") {
        if (!s) s = "";
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
    }
    Text(const Text& other) {
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
    }
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* fresh = new char[std::strlen(other.data) + 1];
            std::strcpy(fresh, other.data);
            delete[] data; data = fresh;
        }
        return *this;
    }
    ~Text() { delete[] data; }
    Text operator+(const Text& rhs) const {
        std::size_t n = std::strlen(data) + std::strlen(rhs.data);
        char* joined = new char[n + 1];
        std::strcpy(joined, data); std::strcat(joined, rhs.data);
        Text result(joined); delete[] joined; return result;
    }
    bool operator==(const Text& rhs) const { return std::strcmp(data, rhs.data) == 0; }
    bool operator!=(const Text& rhs) const { return !(*this == rhs); }
    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.data; }
};
int main() {
    Text a("Hello "), b("Yuvaraj"), empty;
    Text c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c != empty) << '\n';
    a = a; // self-assignment
    Text longMessage("A longer message used to exercise dynamic allocation.");
    std::cout << longMessage << '\n';
}
