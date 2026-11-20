#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Cart {
    std::string items_[10];
    double prices_[10]{};
    int n_ = 0;
    double discount_ = 0;
public:
    Cart& add(const std::string& item, double price) {
        if (n_ < 10 && price >= 0) {
            items_[n_] = item;
            prices_[n_] = price;
            ++n_;
        }
        return *this;
    }
    Cart& setDiscount(double discount) {
        this->discount_ = (discount < 0 ? 0 : (discount > 100 ? 100 : discount));
        return *this;
    }
    double total() const {
        double t = 0;
        for (int i = 0; i < n_; ++i) t += prices_[i];
        return t * (100 - discount_) / 100;
    }
};
int main() {
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c;
    p->add("Oil", 180);
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';

    Cart second;
    second.add("Milk", 50).add("Bread", 40);
    Cart* carts[] = {&c, &second};
    Cart* highest = carts[0];
    for (Cart* cart : carts) if (cart->total() > highest->total()) highest = cart;
    std::cout << "Highest cart total: " << highest->total() << '\n';
}
