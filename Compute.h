#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include "Component.h"
#include "Surface.h"

class Compute : public QObject
{
    Q_OBJECT

public:
    explicit Compute(Component& ui);

private:
    enum class Option {
        PUT,
        CALL
    };

    static constexpr int SAMPLES = 200;

    void recompute();

    Component& ui;
    Surface::SurfaceMode surfaceMode;


    // User-Input Variables
    // Stock Price
    double S;
    double minS;
    double maxS;
    double currentS;
    double deltaS;

    // Strike Price
    double K;
    double minK;
    double maxK;
    double currentK;
    double deltaK;

    // Risk-Free Rate (Annual)
    double r;
    double minr;
    double maxr;
    double currentr;
    double deltar;

    // Dividend Yields (Annual)
    double q;
    double minq;
    double maxq;
    double currentq;
    double deltaq;

    // Volatility (Annual)
    double sigma;
    double minSigma;
    double maxSigma;
    double currentSigma;
    double deltaSigma;

    // Time in Years
    double T;
    double minT;
    double maxT;
    double currentT;
    double deltaT;

    // Computed variables
    double d1; // Result of computeD1
    double d2; // Result of computeD2
    double Nd1; // Result of N(d1)
    double Nd2; // Result of N(d2)
    double C; // Call Price
    double P; // Put Price
    double price; // Selected Price (Either C or P)
};

#endif // COMPUTE_H
