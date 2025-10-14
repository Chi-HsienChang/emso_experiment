#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>

static inline double folded_trap6(int u);

static inline double trap6(int u);

static inline double trap_linear_deceptive(int u, int K, double high, double low);

double calculate_segment_onemax_weak(const std::string& segment, const std::string& method);

double calculate_segment_fitness(const std::string& segment, const std::string& method);

double calculate_segment_fitness_test(const std::string& segment);

double calculate_onemax_weak(const std::string& segment, const std::string& method);

double calculate_fitness(const std::string& chromosome, const std::string& method);

#endif 
