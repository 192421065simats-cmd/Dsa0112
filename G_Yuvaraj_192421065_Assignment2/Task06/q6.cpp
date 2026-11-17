#include <iostream>
#include <cstdlib>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};
int minutesBetween(const Time& a, const Time& b) {
    int first = a.h * 60 + a.m;
    int second = b.h * 60 + b.m;
    return std::abs(first - second);
}
bool earlier(const Time& a, const Time& b) {
    return a.h * 60 + a.m < b.h * 60 + b.m;
}
int main() {
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';
    Time noonA(11, 59), noonB(12, 1), same(12, 1);
    std::cout << minutesBetween(noonA, noonB) << ' '
              << earlier(noonA, noonB) << ' '
              << earlier(same, noonB) << '\n';
}
