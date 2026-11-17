#include <iostream>
#include <string>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false) {
    double amount = weightKg * 40.0;
    if (express) amount += 50.0;
    if (insured) amount += 20.0;
    return amount;
}
double charge(int weightGrams) {
    return charge(static_cast<double>(weightGrams) / 1000.0);
}
double charge(float weightKg) {
    return charge(static_cast<double>(weightKg));
}
double charge(const Parcel& p) {
    return charge(p.weightKg, p.express, p.insured);
}

int main() {
    // Six unambiguous calls and the selected overload are labelled.
    std::cout << "charge(2) [int grams]: " << charge(2) << '\n';
    std::cout << "charge(2.0) [double kg]: " << charge(2.0) << '\n';
    std::cout << "charge(2.0f) [float kg]: " << charge(2.0f) << '\n';
    std::cout << "charge(2000) [int grams]: " << charge(2000) << '\n';
    std::cout << "charge(2.0, true) [double + express]: " << charge(2.0, true) << '\n';
    Parcel p{2.0, true, true};
    std::cout << "charge(Parcel) [object]: " << charge(p) << '\n';
}
