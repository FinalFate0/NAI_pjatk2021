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

vector<double> hillclimb(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, 
    vector<double> p0, vector<double> next_p, int iterations)
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

vector<double> annealing(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, 
    vector<double> p0, int iterations, 
    function<vector<double>(vector<double>)> norm, function<double(int)> temp)
{
    auto s_current = p0;
    auto s_best = p0;

    std::uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    for (int i = 0; i < iterations; i++) {
        vector<double> s_new;
        do {
            s_new = norm(s_current);
        }
        while(!f_domain(s_new));
        if (f(s_new) < f(s_current)) {
            s_current = s_new;
        } else {
            double u = u_k(gen);
            if (u < std::exp(-std::abs(f(s_new) - f(s_current)) / temp(i))) {
                s_current = s_new;
            }
        }
        if (f(s_current) < f(s_best)) {
            s_best = s_current;
        }
        //std::cout << s_current << " -> " << f(s_current) << std::endl;
    }
    return s_best;
}

int main() {

    auto temperature = [](int k){
        return 1000.0 / k;
    };

    auto normal = [](auto p) {
            std::normal_distribution<double> n(0.0, 0.3);
            for (auto& e : p) {
                e = e + n(gen);
            }
            return p;
    };
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

    vector<double> next_p_dist = {-0.1, 0.1};

    int f_input = 0;
    std::cout << "Choose function to optimize:" << std::endl
            << "    [1] Rastrigin function" << std::endl
            << "    [2] Eggholder function  ";
    if(!(std::cin >> f_input) || f_input < 1 || f_input > 2) throw std::logic_error("Invalid input");

    int a_input = 0;
    std::cout << "Choose algorithm for optimization:" << std::endl
            << "    [1] Hillclimb" << std::endl
            << "    [2] Simulated annealing  ";
    if(!(std::cin >> a_input) || a_input < 1 || a_input > 2) throw std::logic_error("Invalid input");

    int iterations = 0;
    std::cout << "Input number of iterations: ";
    if(!(std::cin >> iterations) || iterations <= 0) throw std::logic_error("Invalid input");

    vector<double> result = {0.0};
    switch(f_input) {
        case 1: {
            if(a_input == 1) {
                auto result = hillclimb(rastrigin, rastrigin_domain, rastrigin_p0, next_p_dist, iterations);
                std::cout << result << " -> " << rastrigin(result) << std::endl;
            } else if(a_input == 2) {
                auto result = annealing(rastrigin, rastrigin_domain, rastrigin_p0, iterations, normal, temperature);
                std::cout << std::endl << result << " -> " << rastrigin(result) << std::endl;
            }
            break;
        }

        case 2: {
            if(a_input == 1) {
                auto result = hillclimb(eggholder, eggholder_domain, eggholder_p0, next_p_dist, iterations);
                std::cout << result << " -> " << eggholder(result) << std::endl;
            } else if(a_input == 2) {
                auto result = annealing(eggholder, eggholder_domain, eggholder_p0, iterations, normal, temperature);
                std::cout << std::endl << result << " -> " << eggholder(result) << std::endl;
            }
            break;
        }
        default:
            return -1;
    }
    return 0;
}
