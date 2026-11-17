
#include <chrono>
#include <iostream>
#include <cstdint>

volatile std::uint64_t sink = 0;
struct Plain {
    volatile std::uint64_t value;
    __attribute__((noinline)) std::uint64_t get() const { return value; }
};
struct Inlined {
    volatile std::uint64_t value;
    inline std::uint64_t get() const { return value; }
};
template <typename T>
long long measure(const T& obj, std::uint64_t iterations) {
    auto start = std::chrono::steady_clock::now();
    std::uint64_t sum = 0;
    for (std::uint64_t i = 0; i < iterations; ++i) sum += obj.get();
    sink = sum;
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
int main() {
    constexpr std::uint64_t N = 10000000;
    Plain p{7};
    Inlined q{7};
    std::cout << "Plain non-inlined member calls (microseconds): " << measure(p, N) << '\n';
    std::cout << "Inline member calls (microseconds): " << measure(q, N) << '\n';
    std::cout << "Sink: " << sink << '\n';
}
