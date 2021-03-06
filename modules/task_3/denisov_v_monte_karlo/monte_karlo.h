// Copyright 2020 Vladislav Denisov
#ifndef MODULES_TASK_3_DENISOV_V_MONTE_KARLO_MONTE_KARLO_H_
#define MODULES_TASK_3_DENISOV_V_MONTE_KARLO_MONTE_KARLO_H_

#include <functional>
#include <vector>

double getIntegralMonteCarloPar(const std::function<double(const std::vector<double>&)>& func,
    const std::vector<double>& lowLimInt, const std::vector<double>& highLimInt,
    const unsigned int numberPoints, const int seed);
double getIntegralMonteCarloSeq(const std::function<double(const std::vector<double>&)>& func,
    const std::vector<double>& lowLimInt, const std::vector<double>& highLimInt,
    const unsigned int numberPoints, const int seed);

#endif  // MODULES_TASK_3_DENISOV_V_MONTE_KARLO_MONTE_KARLO_H_
