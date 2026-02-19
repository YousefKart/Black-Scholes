#include "functions.h"
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

double Functions::computeNP(double x) {
    static const double INV_SQRT_2PI = 0.3989422804014327;
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}

double Functions::computeCallPrice(double S, double K, double r, double q, double sigma, double T) {
    const double F = S * std::exp((r-q)*T);
    auto d1 = computeD1(S, K, r, q, sigma, T);
    auto d2 = computeD2(sigma, T, d1);
    auto Nd1 = computeN(d1);
    auto Nd2 = computeN(d2);
    return std::exp(-r*T) * (F*Nd1 - K*Nd2);
}

double Functions::computePutPrice(double S, double K, double r, double q, double sigma, double T) {
    const double F = S * std::exp((r-q)*T);
    auto d1 = computeD1(S, K, r, q, sigma, T);
    auto d2 = computeD2(sigma, T, d1);
    auto Nd1 = computeN(-d1);
    auto Nd2 = computeN(-d2);
    return std::exp(-r*T) * (K*Nd2 - F*Nd1);
}

double Functions::computeCallDelta(double S, double K, double r, double q, double sigma, double T) {
    auto d1 = computeD1(S, K, r, q, sigma, T);
    return std::exp(-q*T) * computeN(d1);
}

double Functions::computePutDelta(double S, double K, double r, double q, double sigma, double T) {
    auto d1 = computeD1(S, K, r, q, sigma, T);
    return std::exp(-q*T) * (computeN(d1) - 1.0);
}

double Functions::computeGamma(double S, double K, double r, double q, double sigma, double T) {
    auto d1 = computeD1(S, K, r, q, sigma, T);
    return std::exp(-q*T) * computeNP(d1) / (S * sigma * std::sqrt(T));
}

double Functions::computeVega(double S, double K, double r, double q, double sigma, double T) {
    auto d1 = computeD1(S, K, r, q, sigma, T);
    return S * std::exp(-q*T) * computeNP(d1) * std::sqrt(T);
}

double Functions::computeCallTheta(double S, double K, double r, double q, double sigma, double T) {
    double d1 = computeD1(S, K, r, q, sigma, T);
    double d2 = computeD2(sigma, T, d1);

    double term1 = - (S * computeNP(d1) * sigma * std::exp(-q*T)) / (2 * std::sqrt(T));
    double term2 = q * S * std::exp(-q*T) * computeN(d1);
    double term3 = - r * K * std::exp(-r*T) * computeN(d2);

    return term1 + term2 + term3;
}

double Functions::computePutTheta(double S, double K, double r, double q, double sigma, double T) {
    double d1 = computeD1(S, K, r, q, sigma, T);
    double d2 = computeD2(sigma, T, d1);

    double term1 = - (S * computeNP(d1) * sigma * std::exp(-q*T)) / (2 * std::sqrt(T));
    double term2 = - q * S * std::exp(-q*T) * computeN(-d1);
    double term3 = r * K * std::exp(-r*T) * computeN(-d2);

    return term1 + term2 + term3;
}

double Functions::computeCallRho(double S, double K, double r, double q, double sigma, double T) {
    double d1 = computeD1(S, K, r, q, sigma, T);
    double d2 = computeD2(sigma, T, d1);

    return K * T * std::exp(-r*T) * computeN(d2);
}

double Functions::computePutRho(double S, double K, double r, double q, double sigma, double T) {
    double d1 = computeD1(S, K, r, q, sigma, T);
    double d2 = computeD2(sigma, T, d1);

    return -K * T * std::exp(-r*T) * computeN(-d2);
}

double Functions::computeCallIV(double S, double K, double r, double q, double P, double T) {
    return 0;
}

double Functions::computePutIV(double S, double K, double r, double q, double P, double T) {
    return 0;
}
