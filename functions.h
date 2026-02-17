#ifndef FUNCTIONSS_H
#define FUNCTIONSS_H

class Functions
{
public:
    Functions();

    // Mains
    static double computeD1(double S, double K, double r, double q, double sigma, double T);
    static double computeD2(double sigma, double T, double d1);
    static double computeN(double x);
    static double computeNP(double x);
    static double computeCallPrice(double S, double K, double r, double q, double sigma, double T);
    static double computePutPrice(double S, double K, double r, double q, double sigma, double T);

    // Greeks
    static double computeCallDelta(double S, double K, double r, double q, double sigma, double T);
    static double computePutDelta(double S, double K, double r, double q, double sigma, double T);
    static double computeGamma(double S, double K, double r, double q, double sigma, double T);
    static double computeVega(double S, double K, double r, double q, double sigma, double T);
    static double computeCallTheta(double S, double K, double r, double q, double sigma, double T);
    static double computePutTheta(double S, double K, double r, double q, double sigma, double T);
    static double computeCallRho(double S, double K, double r, double q, double sigma, double T);
    static double computePutRho(double S, double K, double r, double q, double sigma, double T);

};

#endif // FUNCTIONSS_H
