#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class ShiftHistory {
    std::string data[10];
    int count = 0;
public:
    void add(const std::string& s) {
        if (count < 10) data[count++] = s;
        else {
            for (int i = 1; i < 10; ++i) data[i - 1] = data[i];
            data[9] = s;
        }
    }
    int size() const { return count; }
};

class CircularHistory {
    std::string data[10];
    int start = 0;
    int count = 0;
public:
    void add(const std::string& s) {
        if (count < 10) {
            data[(start + count) % 10] = s;
            ++count;
        } else {
            data[start] = s;
            start = (start + 1) % 10;
        }
    }
    int size() const { return count; }
};

template <typename History>
long long benchmark(History& h, int n) {
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) h.add("transaction-" + std::to_string(i));
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}
int main() {
    constexpr int N = 200000;
    ShiftHistory shifted;
    CircularHistory circular;
    auto a = benchmark(shifted, N);
    auto b = benchmark(circular, N);
    std::cout << "Transactions inserted: " << N << '\n';
    std::cout << "Shift design retained: " << shifted.size() << '\n';
    std::cout << "Circular design retained: " << circular.size() << '\n';
    std::cout << "Shift insertion time (microseconds): " << a << '\n';
    std::cout << "Circular insertion time (microseconds): " << b << '\n';
}
