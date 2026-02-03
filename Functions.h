#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class Functions
{
public:
    Functions();

    static double computeD1(double S, double K, double r, double q, double sigma, double T);
    static double computeD2(double sigma, double T, double d1);
    static double computeN(double x);
    static double computeC(double S, double K, double r, double q, double T, double d1, double d2);
    static double computeP(double S, double K, double r, double q, double T, double d1, double d2);

};

#endif // FUNCTIONS_H
