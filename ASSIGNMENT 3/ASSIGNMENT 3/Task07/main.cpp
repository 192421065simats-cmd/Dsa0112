#include <cmath>
#include <iostream>
#include <stdexcept>

class Complex {
    double re, im;
public:
    Complex() : re(0), im(0) {}
    Complex(double real) : re(real), im(0) {}
    Complex(double real, double imag) : re(real), im(imag) {}
    Complex operator+(const Complex& z) const { return {re + z.re, im + z.im}; }
    Complex operator-(const Complex& z) const { return {re - z.re, im - z.im}; }
    Complex operator*(const Complex& z) const {
        return {re*z.re - im*z.im, re*z.im + im*z.re};
    }
    Complex operator/(const Complex& z) const {
        double d = z.re*z.re + z.im*z.im;
        if (d == 0.0) throw std::domain_error("division by zero impedance");
        return {(re*z.re + im*z.im)/d, (im*z.re - re*z.im)/d};
    }
    bool operator==(const Complex& z) const { return re == z.re && im == z.im; }
    double magnitude() const { return std::hypot(re, im); }
    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        return os << z.re << (z.im < 0 ? " - j" : " + j") << std::abs(z.im);
    }
};
int main() {
    const double pi = std::acos(-1.0), f = 50.0, w = 2*pi*f;
    const double R = 100.0, L = 0.1, C = 100e-6;
    Complex zr(R, 0), zl(0, w*L), zc(0, -1.0/(w*C));
    Complex series = zr + zl + zc;
    Complex parallel = (zr * zl) / (zr + zl);
    std::cout << "Resistor: " << zr << ", |Z|=" << zr.magnitude() << '\n';
    std::cout << "Inductor: " << zl << ", |Z|=" << zl.magnitude() << '\n';
    std::cout << "Capacitor: " << zc << ", |Z|=" << zc.magnitude() << '\n';
    std::cout << "Series RLC: " << series << ", |Z|=" << series.magnitude() << '\n';
    std::cout << "Parallel R-L: " << parallel << ", |Z|=" << parallel.magnitude() << '\n';
    try { Complex bad = zr / Complex(); (void)bad; }
    catch (const std::domain_error& e) { std::cout << "Safe error: " << e.what() << '\n'; }
}
