#include <iostream>
#include <vector>
#include <functional>
#include <tuple>

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
    return [max_x, max_y];
};

int a = 10;


int main() {
     auto wynik = obliczenia(f1);


}

