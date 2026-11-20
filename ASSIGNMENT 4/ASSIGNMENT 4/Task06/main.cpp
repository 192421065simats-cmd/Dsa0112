#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Vehicle {
    std::string registration_;
    double dailyRate_;
    bool rented_ = false;
public:
    Vehicle(std::string reg, double rate)
        : registration_(std::move(reg)), dailyRate_(rate) {}
    virtual ~Vehicle() = default;
    const std::string& registration() const { return registration_; }
    double dailyRate() const { return dailyRate_; }
    bool rented() const { return rented_; }
    void setRented(bool value) { rented_ = value; }
    virtual double charge(int days) const = 0;
    virtual void list() const = 0;
};
class Bicycle : public Vehicle {
public:
    Bicycle(std::string reg, double rate) : Vehicle(std::move(reg), rate) {}
    double charge(int days) const override { return dailyRate() * days; }
    void list() const override { std::cout << "Bicycle " << registration() << " rate=" << dailyRate() << '\n'; }
};
class MotorVehicle : public Vehicle {
    double level_;
    std::string serviceDue_;
public:
    MotorVehicle(std::string reg, double rate, double level, std::string due)
        : Vehicle(std::move(reg), rate), level_(level), serviceDue_(std::move(due)) {}
    double level() const { return level_; }
    const std::string& serviceDue() const { return serviceDue_; }
};
class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(std::string reg, double rate, double fuel, std::string due)
        : MotorVehicle(std::move(reg), rate, fuel, std::move(due)) {}
    double charge(int days) const override { return dailyRate() * days; }
    void list() const override {
        std::cout << "TwoWheeler " << registration() << " fuel=" << level()
                  << " service=" << serviceDue() << " rate=" << dailyRate() << '\n';
    }
};
class Car : public MotorVehicle {
    int seats_;
public:
    Car(std::string reg, double rate, double fuel, std::string due, int seats)
        : MotorVehicle(std::move(reg), rate, fuel, std::move(due)), seats_(seats) {}
    double charge(int days) const override { return dailyRate() * days; }
    void list() const override {
        std::cout << "Car " << registration() << " seats=" << seats_
                  << " fuel=" << level() << " rate=" << dailyRate() << '\n';
    }
};
class ElectricCar : public Car {
    double batteryKWh_;
    double pricePerKWh_;
public:
    ElectricCar(std::string reg, double rate, double chargeLevel, std::string due,
                int seats, double battery, double pricePerKWh)
        : Car(std::move(reg), rate, chargeLevel, std::move(due), seats),
          batteryKWh_(battery), pricePerKWh_(pricePerKWh) {}
    double charge(int days) const override {
        return dailyRate() * days + batteryKWh_ * pricePerKWh_ * 0.0;
    }
    void list() const override {
        Car::list();
        std::cout << "  Electric battery=" << batteryKWh_ << "kWh price/kWh=" << pricePerKWh_ << '\n';
    }
};
class ElectricBus : public MotorVehicle {
    int seats_;
public:
    ElectricBus(std::string reg, double rate, double charge, std::string due, int seats)
        : MotorVehicle(std::move(reg), rate, charge, std::move(due)), seats_(seats) {}
    double charge(int days) const override { return dailyRate() * days; }
    void list() const override {
        std::cout << "ElectricBus " << registration() << " seats=" << seats_
                  << " charge-level=" << level() << " rate=" << dailyRate() << '\n';
    }
};
class Fleet {
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
public:
    template<class T, class... Args> T& add(Args&&... args) {
        auto p = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *p; vehicles_.push_back(std::move(p)); return ref;
    }
    bool rent(const std::string& reg) {
        for (auto& v : vehicles_) if (v->registration() == reg && !v->rented()) {
            v->setRented(true); return true;
        }
        return false;
    }
    double returnVehicle(const std::string& reg, int daysLate) {
        for (auto& v : vehicles_) if (v->registration() == reg && v->rented()) {
            v->setRented(false);
            return v->charge(daysLate < 0 ? 0 : daysLate) + (daysLate > 0 ? daysLate * 100.0 : 0.0);
        }
        return -1.0;
    }
    void list() const { for (const auto& v : vehicles_) v->list(); }
};
int main() {
    Fleet fleet;
    fleet.add<Bicycle>("BI-101", 100);
    fleet.add<TwoWheeler>("TW-202", 250, 5.0, "2027-01-10");
    fleet.add<Car>("CA-303", 1200, 20.0, "2027-02-01", 5);
    fleet.add<ElectricCar>("EV-404", 1500, 80.0, "2027-03-01", 5, 60.0, 8.0);
    fleet.add<ElectricBus>("EB-505", 3000, 90.0, "2027-04-01", 30); // extension
    fleet.list();
    std::cout << std::boolalpha << "Rent CA-303: " << fleet.rent("CA-303") << '\n';
    std::cout << "Return CA-303 after 2 late days: Rs "
              << fleet.returnVehicle("CA-303", 2) << '\n';
}
