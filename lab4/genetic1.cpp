#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <functional>

using std::vector;
using std::function;

std::random_device rd;
std::mt19937 gen(rd());

std::ostream& operator<<(std::ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}


int main() {

    auto eggholder = [](vector<double> v){
        double x = v.at(0), y = v.at(1);
        return -(y+47.0)*sin(sqrt(std::abs((x/2.0)+(y+47.0))))-x*sin(sqrt(std::abs(x-(y+47.0))));
    };

    auto eggholder_domain = [](vector<double> v) {
        return (std::abs(v[0]) <= 512.0) && (std::abs(v[1]) <= 512.0);
    };

    std::uniform_real_distribution<> eggholder_dist(-512, 512);

    vector<double> eggholder_p0 = {
        eggholder_dist(gen),
        eggholder_dist(gen)
    };

    return 0;
}
