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

vector<double> hillclimb(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, vector<double> next_p, int iterations)
{
    auto p_current = p0;
    std::uniform_int_distribution<> distrib(0, p_current.size() - 1);
    std::uniform_real_distribution<> distrib_r(next_p[0], next_p[1]);

    if (!f_domain(p_current)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        vector<double> p_new;

        do {
            p_new = p_current;
            p_new[distrib(gen)] += distrib_r(gen);
        }
        while(!f_domain(p_new));
        //std::cout << p << std::endl;
        double y2 = f(p_new);
        if (y2 < f(p_current)) {
            p_current = p_new;
        }
    }
    return p_current;
}

int main() {


    auto rastrigin = [](vector<double> v){
        auto n = (double)v.size();
        double a = 10.0;
        double result = 0.0;
        for(auto &x : v) {
            result += pow(x, 2.0) - 10.0*cos(2.0*M_PI*x);
        }
        return a*n+result;
    };

    auto eggholder = [](vector<double> v){
        double x = v.at(0), y = v.at(1);
        return -(y+47.0)*sin(sqrt(std::abs((x/2.0)+(y+47.0))))-x*sin(sqrt(std::abs(x-(y+47.0))));
    };

    auto rastrigin_domain = [](vector<double> v) {
        return (std::abs(v[0]) <= 5.12) && (std::abs(v[1]) <= 5.12);
    };

    auto eggholder_domain = [](vector<double> v) {
        return (std::abs(v[0]) <= 512.0) && (std::abs(v[1]) <= 512.0);
    };

    std::uniform_real_distribution<> rastrigin_dist(-5.12, 5.12);
    std::uniform_real_distribution<> eggholder_dist(-512, 512);

    vector<double> rastrigin_p0 = {
        rastrigin_dist(gen),
        rastrigin_dist(gen)
    };

    vector<double> eggholder_p0 = {
        eggholder_dist(gen),
        eggholder_dist(gen)
    };

    vector<double> next_p_dist = {-0.01, 0.01};

    int input = 0;
    std::cout << "Choose function to optimize:" << std::endl
            << "    [1] Rastrigin function" << std::endl
            << "    [2] Eggholder function  ";
    if(!(std::cin >> input) || input < 1 || input > 2) throw std::logic_error("Invalid input");

    int iterations = 0;
    std::cout << "Input number of iterations: ";
    if(!(std::cin >> iterations) || iterations <= 0) throw std::logic_error("Invalid input");

    switch(input) {
        case 1: {
            auto result = hillclimb(rastrigin, rastrigin_domain, rastrigin_p0, next_p_dist, iterations);
            std::cout << result << " -> " << rastrigin(result) << std::endl;
            break;
        }

        case 2: {
            auto result = hillclimb(eggholder, eggholder_domain, eggholder_p0, next_p_dist, iterations);
            std::cout << result << " -> " << eggholder(result) << std::endl;
            break;
        }
        default:
            return -1;
    }
    return 0;
}
