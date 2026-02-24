//
// Created by liad on 24/02/2026.
//

#include "RegenXUtil.hpp"
#include <cmath>
#include <limits>
#include <random>

std::random_device rd;  // Seed for random number generator
std::mt19937 gen(rd()); // Mersenne Twister RNG

namespace regex::utill
{
    double randouble(const double min, const double max)
    {
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };
    float randfloat(const float min, const float max)
    {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    };
    int randint(const int min, const int max)
    {
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    };
    bool randbool()
    {
        std::bernoulli_distribution dis(0.5); // 50% chance of true or false
        return dis(gen);
    };
}
