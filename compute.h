#ifndef COMPUTEE_H
#define COMPUTEE_H

#include <QObject>
#include "component.h"
#include "surface.h"

class Compute : public QObject
{
    Q_OBJECT

public:
    explicit Compute(Component& ui);

private:
    static constexpr int SAMPLES = 200;

    void recompute();
    void setUI(Surface::SurfaceConfig config); // Updates active UI
    void bindLinear(QSlider* slider, QDoubleSpinBox* spin, double min, double max); // Binds a slider to a spin box linearly
    void bindRangeLinear(RangeSlider* slider, QDoubleSpinBox* spinMin, QDoubleSpinBox* spinMax, double min, double max);
    void bindLog(QSlider* slider, QDoubleSpinBox* spin, double min, double max); // Binds a slider to a spin box logarithmically
    void bindRangeLog(RangeSlider* slider, QDoubleSpinBox* spinMin, QDoubleSpinBox* spinMax, double min, double max);

    Component& ui;
    Surface::SurfaceMode surfaceMode;
    Surface::SurfaceConfig config;

    // Stock Price
    double S;
    double min_S;
    double max_S;

    // Strike Price
    double K;
    double min_K;
    double max_K;

    // Risk-Free Rate (Annual)
    double r;
    double min_r;
    double max_r;

    // Dividend Yields (Annual)
    double q;
    double min_q;
    double max_q;

    // Volatility (Annual)
    double sigma;
    double min_sigma;
    double max_sigma;

    // Time in Years
    double T;
    double min_T;
    double max_T;

    // Computed variables
    double d1; // Result of computeD1
    double d2; // Result of computeD2
    double Nd1; // Result of N(d1)
    double Nd2; // Result of N(d2)
    double C; // Call Price
    double P; // Put Price
    double price; // Selected Price (Either C or P)
};

#endif // COMPUTEE_H
