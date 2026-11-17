#include <iostream>
#include <iomanip>

void change(int a, int& b, int* c) {
    a += 10;
    b += 10;
    *c += 10;
}
int& bigger(int& p, int& q) { return p > q ? p : q; }
void show(int x, int y = 5, int z = 10) {
    std::cout << x + y + z << ' ';
}

void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100;
}
void applyConcession(long long& fee, int percent) {
    fee -= fee * percent / 100;
}
void applyByPointer(long long* fee, int percent) {
    if (fee != nullptr) *fee -= *fee * percent / 100;
}
long long returnedConcession(long long fee, int percent) {
    return fee - fee * percent / 100;
}

int main() {
    std::cout << "PREDICTED OUTPUT (before execution):\n"
              << "1 12 13\n1 100\n16 13 6\n";
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << "OBSERVED OUTPUT:\n";
    std::cout << a << ' ' << b << ' ' << c << '\n';
    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';
    show(1); show(1, 2); show(1, 2, 3);
    std::cout << '\n';

    long long fee = 45000;
    applyByValue(fee, 10);
    std::cout << "after call by value   : " << fee << '\n';
    applyConcession(fee, 10);
    std::cout << "after call by reference: " << fee << '\n';
    applyByPointer(&fee, 10);
    std::cout << "after call by pointer  : " << fee << '\n';
    std::cout << "returned new fee       : " << returnedConcession(fee, 10) << '\n';
}
