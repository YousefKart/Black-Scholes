#include "Compute.h"
#include "Functions.h"

Compute::Compute(Component& ui) :
    ui(ui),
    surfaceMode(Surface::SurfaceMode::STP),
    S(0), min_S(0), max_S(0), current_S(0), delta_S(0),
    K(0), min_K(0), max_K(0), current_K(0), delta_K(0),
    r(0), min_r(0), max_r(0), current_r(0), delta_r(0),
    q(0), min_q(0), max_q(0), current_q(0), delta_q(0),
    sigma(0), min_sigma(0), max_sigma(0), current_sigma(0), delta_sigma(0),
    T(0), min_T(0), max_T(0), current_T(0), delta_T(0),
    d1(0), d2(0), Nd1(0), Nd2(0), C(0), P(0), price(0)
{
    // Recompute on update
    QObject::connect(ui.buttonGroup(), &QButtonGroup::idClicked, &ui, [this](int id) {surfaceMode = static_cast<Surface::SurfaceMode>(id); recompute();});
    QObject::connect(ui.toggle_CP(), &QPushButton::toggled, [this]{recompute();});

    bindLog(ui.slider_S(), ui.spin_S(), Component::minValue_S, Component::maxValue_S);
    bindLog(ui.slider_K(), ui.spin_K(), Component::minValue_K, Component::maxValue_K);
    bindLinear(ui.slider_r(), ui.spin_r(), Component::minValue_r, Component::maxValue_r);
    bindLinear(ui.slider_sigma(), ui.spin_sigma(), Component::minValue_sigma, Component::maxValue_sigma);

    QObject::connect(ui.spinmin_T(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinmax_T(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});

    recompute();
}

void Compute::recompute() {
    Option mode = ui.toggle_CP()->isChecked() ? Option::PUT : Option::CALL;

    // Variables
    S = ui.spin_S()->value();
    K = ui.spin_K()->value();
    r = ui.spin_r()->value() / 100.0; // Convert from % to 0.0-1.0 scale
    q = 0.0; // TODO: Add dividend yields
    sigma = ui.spin_sigma()->value();
    min_T = ui.spinmin_T()->value() / 365.25; // Convert from days to years
    max_T = ui.spinmax_T()->value() / 365.25; // Convert from days to years
    T = max_T - min_T;

    // Functions
    d1 = Functions::computeD1(S, K, r, q, sigma, T);
    d2 = Functions::computeD2(sigma, T, d1);
    Nd1 = Functions::computeN(d1);
    Nd2 = Functions::computeN(d2);
    C = Functions::computeC(S, K, r, q, T, d1, d2);
    P = Functions::computeP(S, K, r, q, T, d1, d2);

    // Change in Stock Price
    current_S = S, min_S = 0.5*S, max_S = 1.5*S;
    delta_S = (max_S - min_S) / (SAMPLES - 1);

    // Change in Strike Price
    current_K = K, min_K = 0.5*K, max_K = 1.5*K;
    delta_K = (max_K - min_K) / (SAMPLES - 1);

    // Change in Risk-Free Rate
    current_r = r, min_r = 0.5*r, max_r = 1.5*r;
    delta_r = (max_r - min_r) / (SAMPLES - 1);

    // Change in Dividend Yields
    current_q = q, min_q = 0.5*q, max_q = 1.5*q;
    delta_q = (max_q - min_q) / (SAMPLES - 1);

    // Change in Volatility
    current_sigma = sigma, min_sigma = 0.5*sigma, max_sigma = 1.5*sigma;
    delta_sigma = (max_sigma - min_sigma) / (SAMPLES - 1);

    // Change in Time
    current_T = T;
    delta_T = (max_T - min_T) / (SAMPLES - 1);

    QCPColorMapData *mapData = ui.colorMap()->data();
    mapData->setSize(SAMPLES, SAMPLES);
    mapData->setRange(QCPRange(min_T, max_T), QCPRange(min_S, max_S));

    auto computeOption = mode == Option::PUT ? Functions::computeP : Functions::computeC;

    for (int i = 0; i < SAMPLES; ++i) {
        switch (surfaceMode) {
        case Surface::SurfaceMode::STP:
            current_T += delta_T;
            break;
        case Surface::SurfaceMode::SSP:
            current_sigma += delta_sigma;
            break;
        default:
            break;
        }

        current_S = min_S;

        for (int j = 0; j < SAMPLES; ++j) {
            current_S += delta_S;

            d1 = Functions::computeD1(current_S, current_K, current_r, current_q, current_sigma, current_T);
            d2 = Functions::computeD2(current_sigma, current_T, d1);

            price = computeOption(current_S, current_K, current_r, current_q, current_T, d1, d2);
            mapData->setCell(i, j, price);
        }
    }

    ui.toggle_CP()->setText(mode == Option::PUT ? "Mode: Puts" : "Mode: Calls");
    ui.colorScale()->axis()->setLabel(mode == Option::PUT ? "Put Price" : "Call Price");
    ui.colorMap()->rescaleDataRange(true);
    ui.plot()->xAxis->setLabel(surfaceMode == Surface::SurfaceMode::STP ? "Years (T)" : "Volatility (\u03C3)");
    ui.plot()->yAxis->setLabel("Stock Price (S)");
    ui.plot()->xAxis->setRange(min_T, max_T);
    ui.plot()->yAxis->setRange(min_S, max_S);
    ui.plot()->replot();
}

void Compute::bindLinear(QSlider* slider, QDoubleSpinBox* spin, double min, double max) {
    QObject::connect(slider, &QSlider::valueChanged, this, [this, slider, spin, min, max](int value) {
        spin->blockSignals(true);
        spin->setValue(Component::sliderToSpinLinear(value, min, max, Component::SLIDER_RESOLUTION));
        spin->blockSignals(false);
        recompute();
    });

    QObject::connect(spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spin, min, max](double value){
        slider->blockSignals(true);
        slider->setValue(Component::spinToSliderLinear(value, min, max, Component::SLIDER_RESOLUTION));
        slider->blockSignals(false);
        recompute();
    });
}

void Compute::bindLog(QSlider* slider, QDoubleSpinBox* spin, double min, double max) {
    QObject::connect(slider, &QSlider::valueChanged, this, [this, slider, spin, min, max](int value) {
        spin->blockSignals(true);
        spin->setValue(Component::sliderToSpinLog(value, min, max, Component::SLIDER_RESOLUTION));
        spin->blockSignals(false);
        recompute();
    });

    QObject::connect(spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spin, min, max](double value){
        slider->blockSignals(true);
        slider->setValue(Component::spinToSliderLog(value, min, max, Component::SLIDER_RESOLUTION));
        slider->blockSignals(false);
        recompute();
    });
}
