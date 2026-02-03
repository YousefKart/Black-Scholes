#include "Functions.h"
#include <cmath>

Functions::Functions() {}

double Functions::computeD1(double S, double K, double r, double q, double sigma, double T) {
    const double num = std::log(S/K) + (r - q + 0.5*sigma*sigma) * T;
    const double den = sigma * std::sqrt(T);
    return num / den;
}

double Functions::computeD2(double sigma, double T, double d1) {
    return d1 - sigma*std::sqrt(T);
}

double Functions::computeN(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double Functions::computeC(double S, double K, double r, double q, double T, double d1, double d2) {
    const double F = S * std::exp((r-q)*T);
    auto Nd1 = computeN(d1);
    auto Nd2 = computeN(d2);
    return std::exp(-r*T) * (F*Nd1 - K*Nd2);
}

double Functions::computeP(double S, double K, double r, double q, double T, double d1, double d2) {
    const double F = S * std::exp((r-q)*T);
    auto Nd1 = computeN(-d1);
    auto Nd2 = computeN(-d2);
    return std::exp(-r*T) * (K*Nd2 - F*Nd1);
}
