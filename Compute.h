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
    void bindLinear(QSlider* slider, QDoubleSpinBox* spin, double min, double max);
    void bindLog(QSlider* slider, QDoubleSpinBox* spin, double min, double max);

    Component& ui;
    Surface::SurfaceMode surfaceMode;

    // Stock Price
    double S;
    double min_S;
    double max_S;
    double current_S;
    double delta_S;

    // Strike Price
    double K;
    double min_K;
    double max_K;
    double current_K;
    double delta_K;

    // Risk-Free Rate (Annual)
    double r;
    double min_r;
    double max_r;
    double current_r;
    double delta_r;

    // Dividend Yields (Annual)
    double q;
    double min_q;
    double max_q;
    double current_q;
    double delta_q;

    // Volatility (Annual)
    double sigma;
    double min_sigma;
    double max_sigma;
    double current_sigma;
    double delta_sigma;

    // Time in Years
    double T;
    double min_T;
    double max_T;
    double current_T;
    double delta_T;

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
