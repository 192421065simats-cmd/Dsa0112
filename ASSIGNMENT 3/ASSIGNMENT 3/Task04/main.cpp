#include <iomanip>
#include <iostream>
#include <stdexcept>

class Money {
    long long paise;
public:
    explicit Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    Money operator+(const Money& rhs) const { Money result; result.paise = paise + rhs.paise; return result; }
    Money operator-(const Money& rhs) const { Money result; result.paise = paise - rhs.paise; return result; }
    Money& operator+=(const Money& rhs) { paise += rhs.paise; return *this; }
    bool operator==(const Money& rhs) const { return paise == rhs.paise; }
    bool operator<(const Money& rhs) const { return paise < rhs.paise; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long whole = m.paise / 100, fraction = m.paise % 100;
        if (fraction < 0) fraction = -fraction;
        return os << "Rs " << whole << '.' << std::setw(2) << std::setfill('0') << fraction
                  << std::setfill(' ');
    }
    friend std::istream& operator>>(std::istream& is, Money& m) {
        long long rupees; int p;
        if (is >> rupees >> p) m.paise = rupees * 100 + p;
        return is;
    }
};
int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
    Money carry(0, 75); carry += Money(0, 50);
    std::cout << carry << '\n';
    std::cout << (Money(1) - Money(2)) << " (negative amounts are permitted)\n";
}
