#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class Functions
{
public:
    Functions();

    static double computeD1(double S, double K, double r, double q, double sigma, double T);
    static double computeD2(double d1, double sigma, double T);
    static double computeN(double x);
};

#endif // FUNCTIONS_H
