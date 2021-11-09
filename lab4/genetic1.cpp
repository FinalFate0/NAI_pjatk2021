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


double fitness_egg(ftype_t fenotype) {
    double target_x = 512.0, target_y = 404.2319;
    double candidate_x = fenotype.first, candidate_y = fenotype.second;

    double temp_x = (candidate_x+512.0)/(target_x+512.0);
    double temp_y = (candidate_y+512.0)/(target_y+512.0);

    if(temp_x > 1.0) {
        temp_x -= 2.0*(temp_x-1.0);
    }
    if(temp_y > 1.0) {
        temp_y -= 2.0*(temp_y-1.0);
    }

    double result = (temp_x+temp_y)/2.0;
    return result;
}


//x: 10100101 -> x' = 165 -> real -> 5.12*(x'/(128))
//y: 11011101 -> y' = 221 -> real -> 5.12*(y'/(128))

/*

for (int i = 0; i < 64;i++) {
    x = (x << 1) + n[i];
    y = (y << 1) + n[i+64];
}


*/

ftype_t gen_to_fen(gtype_t genotype)
{
    long long temp_x = 0, temp_y = 0;

    for (int i = 0; i < 64; i++)
    {
        temp_x = (temp_x << 1);
        temp_x += genotype[i];
        temp_y = (temp_y << 1);
        temp_y += genotype[i + 64];
    }
    //std::cout << temp_x << " " << temp_y << std::endl;
    double x, y;
    x = 512.000000000001 * (temp_x / pow(2, 63));
    y = 512.000000000001 * (temp_y / pow(2, 63));

    return std::make_pair(x, y);
}

gtype_t fen_to_gen(ftype_t fenotype)
{
    gtype_t genotype(128, 0);

    long long x = 0.0, y = 0.0;
    x = (long long)((fenotype.first * pow(2, 63)) / 512.000000000001);
    y = (long long)((fenotype.second * pow(2, 63)) / 512.000000000001);

    //std::cout << x << " " << y << std::endl;
    long long temp_x, temp_y;
    for (int i = 63; i >= 0; i--)
    {
        
        temp_x = (x & 1);
        x = x >> 1;
        genotype[i] = temp_x;
        temp_y = (y & 1);
        y = y >> 1;
        genotype[i + 64] = temp_y;
    }
    
    return genotype;
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

    ftype_t test_fenotype = std::make_pair(512.0, 404.2319);

    std::cout << std::fixed;
    std::cout << std::setprecision(10);

    std::cout << gen_to_fen(fen_to_gen(test_fenotype)).first << " " << gen_to_fen(fen_to_gen(test_fenotype)).second << std::endl;
    std::cout << "fitness:" << fitness_egg(test_fenotype) << std::endl; 

    return 0;
}
