#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

class Bill { public: virtual ~Bill()=default; virtual double total() const=0; virtual std::string type() const=0; };
class OutPatient final: public Bill {
 double consultation, tests;
public: OutPatient(double c,double t):consultation(c),tests(t){if(c<0||t<0)throw std::invalid_argument("negative");}
 double total()const override{return consultation+tests;} std::string type()const override{return "OP";}
};
// New bill type added without editing existing bill implementations.
class TelemedicineBill final: public Bill {
 double consultation, platform;
public: TelemedicineBill(double c,double p):consultation(c),platform(p){if(c<0||p<0)throw std::invalid_argument("negative");}
 double total()const override{return consultation+platform;} std::string type()const override{return "Telemedicine";}
};
class Payer { public: virtual ~Payer()=default; virtual double amountCovered(const Bill&)const=0; };
class SelfPay final: public Payer { public: double amountCovered(const Bill& b)const override{return b.total();} };
// New payer extension.
class CorporatePayer final: public Payer {
 double discount;
public: explicit CorporatePayer(double d):discount(d){if(d<0||d>100)throw std::invalid_argument("discount must be 0..100");}
 double amountCovered(const Bill& b)const override{return b.total()*(1.0-discount/100.0);}
};
int main(){
 std::vector<std::unique_ptr<Bill>> bills;
 bills.emplace_back(std::make_unique<OutPatient>(500,1200));
 bills.emplace_back(std::make_unique<TelemedicineBill>(800,100));
 SelfPay self; CorporatePayer corporate(10);
 for(const auto& b:bills) std::cout<<b->type()<<" total="<<b->total()<<" selfpay_cover="<<self.amountCovered(*b)<<" corporate_cover="<<corporate.amountCovered(*b)<<'\n';
}
