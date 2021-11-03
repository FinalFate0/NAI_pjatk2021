#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <functional>
#include <tuple>
//#include <bit>

using std::vector;
using std::pair;
using std::function;

using gtype_t = vector<char>;
using ftype_t = pair<double, double>;

std::random_device rd;
std::mt19937 gen(rd());

std::ostream& operator<<(std::ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}


//x: 10100101 -> x' = 165 -> real -> 5.12*(x'/(128))
//y: 11011101 -> y' = 221 -> real -> 5.12*(y'/(128))

/*

for (int i = 0; i < 64;i++) {
    x = (x << 1) + n[i];
    y = (y << 1) + n[i+64];
}


*/

 ftype_t gen_to_fen(gtype_t genotype) {
    int temp_x = 0, temp_y = 0;


    for (int i = 0; i < 64;i++) {
        temp_x = (temp_x << 1);
        temp_x += genotype[i];
        temp_y = (temp_y << 1);
        temp_y += genotype[i+64];
    }
    std::cout << temp_x << " " << temp_y << std::endl;

    double x, y;
    x = 5.12*temp_x/128;
    y = 5.12*temp_y/128;

    
    return std::make_pair(x, y);
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


    gtype_t genotype(128, 0);
    genotype[127] = 1;
    genotype[125] = 1;
    genotype[124] = 1;
    genotype[123] = 1;
    genotype[122] = 1;
    genotype[121] = 1;
    genotype[120] = 1;

    genotype[63] = 1;
    genotype[61] = 1;
    genotype[60] = 1;
    genotype[59] = 1;
    genotype[58] = 1;
    genotype[57] = 1;
    genotype[56] = 1;

    std::cout << gen_to_fen(genotype).first << " " << gen_to_fen(genotype).second << std::endl;

    return 0;
}
