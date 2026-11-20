#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
volatile std::size_t sink_count=0;
template<class F> long long measure(F f) {
 auto s=std::chrono::steady_clock::now(); sink_count=f();
 return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-s).count();
}
int main() {
 constexpr std::size_t N=1000000;
 for(int invalidPct: {0,1,50}) {
   std::vector<int> values(N,1);
   for(std::size_t i=0;i<N*static_cast<std::size_t>(invalidPct)/100;++i) values[i]=-1;
   auto rc=measure([&]{std::size_t bad=0; for(int x:values) if(x<0) ++bad; return bad;});
   auto ex=measure([&]{std::size_t bad=0; for(int x:values) try { if(x<0) throw std::invalid_argument("bad"); } catch(const std::invalid_argument&) { ++bad; } return bad;});
   std::cout<<invalidPct<<"% invalid: return_code_ms="<<rc<<" exceptions_ms="<<ex<<" invalid_count="<<sink_count<<'\n';
 }
}
