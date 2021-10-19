#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <functional>

using std::vector;
using std::function;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

std::ostream& operator<<(std::ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}

vector<double> hillclimb(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, vector<double> next_p, int iterations)
{
    auto p = p0;
    std::uniform_int_distribution<> distrib(0, p.size() - 1);
    //std::uniform_real_distribution<> distrib_r(-0.1, 0.1);
    std::uniform_real_distribution<> distrib_r(next_p[0], next_p[1]);

    if (!f_domain(p)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto p2 = p;

        p[distrib(gen)] += distrib_r(gen);
        double y2 = f(p2);
        if (y2 < f(p)) {
            p = p2;
        }
    }
    return p;
}

int main(int argc, char** argv) {


    auto rastrigin = [](vector<double> v){
        double x = v.at(0);
        double n = 2;
        double a = 10.0;
        double result = 0.0;
        for(int i = 1;i<n;i++) {
            result += pow(x, 2.0) - 10.0*cos(2.0*M_PI*x);
        }
        return a*n+result;
    };

    auto eggholder = [](vector<double> v){
        double x = v.at(0), y = v.at(1);
        return -(y+47)*sin(sqrt(abs((x/2)+(y+47))))-x*sin(sqrt(abs(x-(y+47))));
    };

    auto rastrigin_domain = [](vector<double> v) {
        return (abs(v[0]) <= 5.12);
    };

    auto eggholder_domain = [](vector<double> v) {
        return (abs(v[0]) <= 512) && (abs(v[1]) <= 512);
    };

    std::uniform_real_distribution<> rastrigin_dist(-5.12, 5.12);
    std::uniform_real_distribution<> eggholder_dist(-512, 512);

    vector<double> rastrigin_p0 = {
        //rastrigin_dist(gen),
        0.0
    };

    vector<double> eggholder_p0 = {
        //eggholder_dist(gen),
        //eggholder_dist(gen)
        511.0,
        404.0
    };

    vector<double> next_p_dist = {-0.1, 0.1};

    auto result = hillclimb(eggholder, eggholder_domain, eggholder_p0, next_p_dist, 10000);
    std::cout << result << " -> " << eggholder(result) << std::endl;

    return 0;
}
