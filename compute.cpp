#include "compute.h"

Compute::Compute(Component& ui) :
    ui(ui),
    surfaceMode(Surface::SurfaceMode::STP),
    config(Surface::surfaceMap[surfaceMode]),
    S(0), min_S(0), max_S(0),
    K(0), min_K(0), max_K(0),
    r(0), min_r(0), max_r(0),
    q(0), min_q(0), max_q(0),
    sigma(0), min_sigma(0), max_sigma(0),
    T(0), min_T(0), max_T(0),
    d1(0), d2(0), Nd1(0), Nd2(0), C(0), P(0), price(0)
{
    // Recompute on update
    QObject::connect(ui.buttonGroup(), &QButtonGroup::idClicked, &ui, [this](int id) {
        surfaceMode = static_cast<Surface::SurfaceMode>(id);
        config = Surface::surfaceMap[surfaceMode];
        this->ui.setConfig(config);
        recompute();
    });

    QObject::connect(ui.toggle_CP(), &QPushButton::toggled, [this]{recompute();});

    bindLog(ui.slider_S(), ui.spin_S(), Component::minLimit_S, Component::maxLimit_S);
    bindRangeLog(ui.rangeSlider_S(), ui.spinMin_S(), ui.spinMax_S(), Component::minLimit_S, Component::maxLimit_S);

    bindLog(ui.slider_K(), ui.spin_K(), Component::minLimit_K, Component::maxLimit_K);
    bindRangeLog(ui.rangeSlider_K(), ui.spinMin_K(), ui.spinMax_K(), Component::minLimit_K, Component::maxLimit_K);

    bindLinear(ui.slider_r(), ui.spin_r(), Component::minLimit_r, Component::maxLimit_r);
    bindRangeLinear(ui.rangeSlider_r(), ui.spinMin_r(), ui.spinMax_r(), Component::minLimit_r, Component::maxLimit_r);

    bindLinear(ui.slider_q(), ui.spin_q(), Component::minLimit_q, Component::maxLimit_q);
    bindRangeLinear(ui.rangeSlider_q(), ui.spinMin_q(), ui.spinMax_q(), Component::minLimit_q, Component::maxLimit_q);

    bindLinear(ui.slider_sigma(), ui.spin_sigma(), Component::minLimit_sigma, Component::maxLimit_sigma);
    bindRangeLinear(ui.rangeSlider_sigma(), ui.spinMin_sigma(), ui.spinMax_sigma(), Component::minLimit_sigma, Component::maxLimit_sigma);

    bindLinear(ui.slider_T(), ui.spin_T(), Component::minLimit_T, Component::maxLimit_T);
    bindRangeLinear(ui.rangeSlider_T(), ui.spinMin_T(), ui.spinMax_T(), Component::minLimit_T, Component::maxLimit_T);

    recompute();
}

void Compute::recompute() {
    Surface::OptionMode mode = ui.toggle_CP()->isChecked() ? Surface::OptionMode::PUT : Surface::OptionMode::CALL;

    // Variables
    S = ui.spin_S()->value();
    min_S = ui.spinMin_S()->value();
    max_S = ui.spinMax_S()->value();

    K = ui.spin_K()->value();
    min_K = ui.spinMin_K()->value();
    max_K = ui.spinMax_K()->value();

    r= ui.spin_r()->value() / 100.0; // Convert from % to 0.0-1.0 scale
    min_r = ui.spinMin_r()->value() / 100.0;
    max_r = ui.spinMax_r()->value() / 100.0;

    q = ui.spin_q()->value() / 100.0; // Convert from % to 0.0-1.0 scale
    min_q = ui.spinMin_q()->value() / 100.0;
    max_q = ui.spinMax_q()->value() / 100.0;

    sigma = ui.spin_sigma()->value();
    min_sigma = ui.spinMin_sigma()->value();
    max_sigma = ui.spinMax_sigma()->value();

    T = ui.spin_T()->value() / 365.25; // Convert from days to years
    min_T = ui.spinMin_T()->value() / 365.25;
    max_T = ui.spinMax_T()->value() / 365.25;

    int idx, idy;
    double min_x, max_x;
    double min_y, max_y;
    double params[6] = { S, K, r, q, sigma, T };

    switch (config.xVal) {
    case 'S': idx = 0, min_x = min_S, max_x = max_S; break;
    case 'K': idx = 1, min_x = min_K, max_x = max_K; break;
    case 'R': idx = 2, min_x = min_r, max_x = max_r; break;
    case 'Q': idx = 3, min_x = min_q, max_x = max_q; break;
    case 'I': idx = 4, min_x = min_sigma, max_x = max_sigma; break;
    case 'T': idx = 5, min_x = min_T, max_x = max_T; break;
    }
    switch (config.yVal) {
    case 'S': idy = 0, min_y = min_S, max_y = max_S; break;
    case 'K': idy = 1, min_y = min_K, max_y = max_K; break;
    case 'R': idy = 2, min_y = min_r, max_y = max_r; break;
    case 'Q': idy = 3, min_y = min_q, max_y = max_q; break;
    case 'I': idy = 4, min_y = min_sigma, max_y = max_sigma; break;
    case 'T': idy = 5, min_y = min_T, max_y = max_T; break;
    }

    double delta_x = (max_x - min_x) / (SAMPLES - 1);
    double delta_y = (max_y - min_y) / (SAMPLES - 1);

    QCPColorMapData *mapData = ui.colorMap()->data();
    mapData->setSize(SAMPLES, SAMPLES);
    mapData->setRange(QCPRange(min_x, max_x), QCPRange(min_y, max_y));

    for (int x = 0; x < SAMPLES; ++x) {
        params[idx] = min_x + x * delta_x;
        for (int y = 0; y < SAMPLES; ++y) {
            params[idy] = min_y + y * delta_y;
            price = config.computeZ(mode, params[0], params[1], params[2], params[3], params[4], params[5]);
            mapData->setCell(x, y, price);
        }
    }

    ui.toggle_CP()->setText(mode == Surface::OptionMode::PUT ? "Mode: Puts" : "Mode: Calls");
    ui.colorScale()->axis()->setLabel(mode == Surface::OptionMode::PUT ? "Put Price" : "Call Price");
    ui.colorMap()->rescaleDataRange(true);
    ui.plot()->xAxis->setLabel(config.xLabel);
    ui.plot()->yAxis->setLabel(config.yLabel);
    ui.plot()->xAxis->setRange(min_x, max_x);
    ui.plot()->yAxis->setRange(min_y, max_y);
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

void Compute::bindRangeLinear(RangeSlider* slider, QDoubleSpinBox* spinMin, QDoubleSpinBox* spinMax, double min, double max) {
    QObject::connect(slider, &RangeSlider::lowerValueChanged, this, [this, slider, spinMin, min, max](int value) {
        spinMin->blockSignals(true);
        spinMin->setValue(Component::sliderToSpinLinear(value, min, max, Component::SLIDER_RESOLUTION));
        spinMin->blockSignals(false);
        recompute();
    });

    QObject::connect(slider, &RangeSlider::upperValueChanged, this, [this, slider, spinMax, min, max](int value) {
        spinMax->blockSignals(true);
        spinMax->setValue(Component::sliderToSpinLinear(value, min, max, Component::SLIDER_RESOLUTION));
        spinMax->blockSignals(false);
        recompute();
    });

    QObject::connect(spinMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spinMin, min, max](double value){
        slider->blockSignals(true);
        slider->setLowerValue(Component::spinToSliderLinear(value, min, max, Component::SLIDER_RESOLUTION));
        slider->blockSignals(false);
        recompute();
    });

    QObject::connect(spinMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spinMin, min, max](double value){
        slider->blockSignals(true);
        slider->setUpperValue(Component::spinToSliderLinear(value, min, max, Component::SLIDER_RESOLUTION));
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

void Compute::bindRangeLog(RangeSlider* slider, QDoubleSpinBox* spinMin, QDoubleSpinBox* spinMax, double min, double max) {
    QObject::connect(slider, &RangeSlider::lowerValueChanged, this, [this, slider, spinMin, min, max](int value) {
        spinMin->blockSignals(true);
        spinMin->setValue(Component::sliderToSpinLog(value, min, max, Component::SLIDER_RESOLUTION));
        spinMin->blockSignals(false);
        recompute();
    });

    QObject::connect(slider, &RangeSlider::upperValueChanged, this, [this, slider, spinMax, min, max](int value) {
        spinMax->blockSignals(true);
        spinMax->setValue(Component::sliderToSpinLog(value, min, max, Component::SLIDER_RESOLUTION));
        spinMax->blockSignals(false);
        recompute();
    });

    QObject::connect(spinMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spinMin, min, max](double value){
        slider->blockSignals(true);
        slider->setLowerValue(Component::spinToSliderLog(value, min, max, Component::SLIDER_RESOLUTION));
        slider->blockSignals(false);
        recompute();
    });

    QObject::connect(spinMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, slider, spinMax, min, max](double value){
        slider->blockSignals(true);
        slider->setUpperValue(Component::spinToSliderLog(value, min, max, Component::SLIDER_RESOLUTION));
        slider->blockSignals(false);
        recompute();
    });
}
