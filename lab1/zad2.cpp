#include <iostream>
#include <vector>
#include <functional>
#include <tuple>

auto pair = [](auto x, auto y){
    return std::make_pair (x, y);
};

auto obliczenia = [](auto f1, auto f2, int n){
    auto max_x = f2(0);
    auto max_y = f1(max_x);
    for(int i = 1;i < n;i++) {
        auto x = f2(i);
        auto y = f1(x);
        if(y > max_y) {
            max_x = x;
            max_y = y;
        }
    }
    return pair(max_x, max_y);
};

int main() {

    auto f1 = [](auto x){
        return 20-abs(x);
    };
    auto f2 = [](auto i){
        return (2*(i%2)-1)*i/2;
    };

    int n = 100;

    auto wynik = obliczenia(f1, f2, n);
    std::cout << wynik.first << ":" << wynik.second << std::endl;
    return 0;
}