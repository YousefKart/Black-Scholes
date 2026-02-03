#include "Functions.h"
#include <cmath>

Functions::Functions() {}

double Functions::computeD1(double S, double K, double r, double q, double sigma, double T) {
    return std::log(S/K) + (r - q + 0.5*sigma*sigma * T) / (sigma*std::sqrt(T));
}

double Functions::computeD2(double d1, double sigma, double T) {
    return d1 - sigma*std::sqrt(T);
}

double Functions::computeN(double x) {
    return 0.5 * std::erfc(-x * M_SQRT1_2);
}
