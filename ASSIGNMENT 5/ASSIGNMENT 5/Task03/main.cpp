#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

struct Base { virtual ~Base() = default; virtual std::uint64_t total() const = 0; };
struct OP final : Base { std::uint64_t a,b; OP(std::uint64_t x,std::uint64_t y):a(x),b(y){} std::uint64_t total() const override{return a+b;} };
struct IP final : Base { std::uint64_t days,rate,surgery; IP(std::uint64_t d,std::uint64_t r,std::uint64_t s):days(d),rate(r),surgery(s){} std::uint64_t total() const override{return days*rate+surgery;} };
struct Tagged { int kind; std::uint64_t a,b,c; };
volatile std::uint64_t sink = 0;
template<class F> long long time_ms(F f) {
    auto start=std::chrono::steady_clock::now(); sink=f();
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start).count();
}
int main(int argc,char** argv) {
    std::size_t n= argc>1 ? static_cast<std::size_t>(std::stoull(argv[1])) : 1000000;
    std::vector<OP> ops; std::vector<IP> ips;
    std::vector<std::unique_ptr<Base>> poly;
    std::vector<Tagged> tagged; ops.reserve(n); ips.reserve(n); poly.reserve(n); tagged.reserve(n);
    for(std::size_t i=0;i<n;++i) {
        if(i%2==0) { ops.emplace_back(500,1200); tagged.push_back({0,500,1200,0}); }
        else { ips.emplace_back(3,2500,40000); tagged.push_back({1,3,2500,40000}); }
    }
    // Separate polymorphic objects; use matching sequence and avoid output in timed loop.
    for(std::size_t i=0;i<n;++i) {
        if(i%2==0) poly.emplace_back(std::make_unique<OP>(500,1200));
        else poly.emplace_back(std::make_unique<IP>(3,2500,40000));
    }
    auto tv=time_ms([&]{std::uint64_t s=0; for(const auto& p:poly)s+=p->total(); return s;});
    auto ts=time_ms([&]{std::uint64_t s=0; for(const auto& x:tagged) s += x.kind==0 ? x.a+x.b : x.b*x.c+x.a*0+40000; return s;});
    std::cout<<"n="<<n<<" virtual_ms="<<tv<<" switch_ms="<<ts<<" checksum_virtual="<<sink<<'\n';
    std::cout<<"Note: repeat at 1000, 1000000 and 10000000; report medians from multiple runs.\n";
}
