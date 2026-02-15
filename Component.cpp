#include "Component.h"
#include "Surface.h"

constexpr double INIT_STOCK_PRICE = 100.0;
constexpr double INIT_STRIKE_PRICE = 110.0;
constexpr double INIT_RISK_FREE_RATE = 10.0;
constexpr double INIT_VOLATILITY = 0.2;
constexpr double INIT_MIN_TIME_TO_EXPIRY = 0.00;
constexpr double INIT_MAX_TIME_TO_EXPIRY = 365.0;

Component::Component(QWidget* parent) : QWidget(parent)
{

    setupPlot(); // Must be called before setupUI()
    setupUI();

    this->setLayout(m_mainLayout);
}

void Component::setupUI() {

    // Left-Hand Menu
    m_menuTitle = new QLabel("Surface Mode", this);
    m_menuTitle->setAlignment(Qt::AlignCenter);

    m_button_STP = new QPushButton("(S,T) -> Price", this);
    m_button_STP->setCheckable(true);
    m_button_STP->setMinimumWidth(150);
    m_button_STP->setMaximumWidth(150);
    m_button_STP->setChecked(true);

    m_button_SSP = new QPushButton(QString::fromUtf8(u8"(S,\u03C3) -> Price"), this);
    m_button_SSP->setCheckable(true);
    m_button_SSP->setMinimumWidth(150);
    m_button_SSP->setMaximumWidth(150);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    m_buttonGroup->addButton(m_button_STP, static_cast<int>(Surface::SurfaceMode::STP));
    m_buttonGroup->addButton(m_button_SSP, static_cast<int>(Surface::SurfaceMode::SSP));

    // Display Type (Calls/Puts)
    m_toggle_CP = new QPushButton(this);
    m_toggle_CP->setCheckable(true);

    // Stock Price
    m_slider_S = new QSlider(Qt::Horizontal, this);
    m_slider_S->setRange(0, SLIDER_RESOLUTION);
    m_slider_S->setValue(spinToSliderLog(INIT_STOCK_PRICE, minValue_S, maxValue_S, SLIDER_RESOLUTION));

    m_spin_S = new QDoubleSpinBox(this);
    m_spin_S->setRange(minValue_S, maxValue_S);
    m_spin_S->setDecimals(2);
    m_spin_S->setSingleStep(0.01);
    m_spin_S->setValue(INIT_STOCK_PRICE);
    m_spin_S->setPrefix("S = ");
    m_spin_S->setMinimumWidth(150);
    m_spin_S->setMaximumWidth(150);

    m_layout_S = new QHBoxLayout();
    m_layout_S->addWidget(m_slider_S);
    m_layout_S->addWidget(m_spin_S);

    // Strike Price
    m_slider_K = new QSlider(Qt::Horizontal, this);
    m_slider_K->setRange(0,SLIDER_RESOLUTION);
    m_slider_K->setValue(spinToSliderLog(INIT_STRIKE_PRICE, minValue_K, maxValue_K, SLIDER_RESOLUTION));

    m_spin_K = new QDoubleSpinBox(this);
    m_spin_K->setRange(minValue_K, maxValue_K);
    m_spin_K->setDecimals(2);
    m_spin_K->setSingleStep(0.01);
    m_spin_K->setValue(INIT_STRIKE_PRICE);
    m_spin_K->setPrefix("K = ");
    m_spin_K->setMinimumWidth(150);
    m_spin_K->setMaximumWidth(150);

    m_layout_K = new QHBoxLayout();
    m_layout_K->addWidget(m_slider_K);
    m_layout_K->addWidget(m_spin_K);

    // Risk-Free Rate (Annual)
    m_slider_r = new QSlider(Qt::Horizontal, this);
    m_slider_r->setRange(0, SLIDER_RESOLUTION);
    m_slider_r->setValue(spinToSliderLinear(INIT_RISK_FREE_RATE, minValue_r, maxValue_r, SLIDER_RESOLUTION));

    m_spin_r = new QDoubleSpinBox(this);
    m_spin_r->setRange(minValue_r, maxValue_r);
    m_spin_r->setDecimals(2);
    m_spin_r->setSingleStep(0.1);
    m_spin_r->setValue(INIT_RISK_FREE_RATE);
    m_spin_r->setPrefix("r = ");
    m_spin_r->setSuffix("%");
    m_spin_r->setMinimumWidth(150);
    m_spin_r->setMaximumWidth(150);

    m_layout_r = new QHBoxLayout();
    m_layout_r->addWidget(m_slider_r);
    m_layout_r->addWidget(m_spin_r);

    // Volatility (Annual)
    m_slider_sigma = new QSlider(Qt::Horizontal, this);
    m_slider_sigma->setRange(0, SLIDER_RESOLUTION);
    m_slider_sigma->setValue(spinToSliderLinear(INIT_VOLATILITY, minValue_sigma, maxValue_sigma, SLIDER_RESOLUTION));

    m_spin_sigma = new QDoubleSpinBox(this);
    m_spin_sigma->setRange(minValue_sigma, maxValue_sigma);
    m_spin_sigma->setDecimals(4);
    m_spin_sigma->setSingleStep(0.01);
    m_spin_sigma->setValue(INIT_VOLATILITY);
    m_spin_sigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));
    m_spin_sigma->setMinimumWidth(150);
    m_spin_sigma->setMaximumWidth(150);

    m_layout_sigma = new QHBoxLayout();
    m_layout_sigma->addWidget(m_slider_sigma);
    m_layout_sigma->addWidget(m_spin_sigma);

    // min_inum Time to Expiry (Days)
    m_spinmin_T = new QDoubleSpinBox(this);
    m_spinmin_T->setRange(minValue_T, maxValue_T);
    m_spinmin_T->setDecimals(2);
    m_spinmin_T->setSingleStep(1);
    m_spinmin_T->setValue(INIT_MIN_TIME_TO_EXPIRY);
    m_spinmin_T->setPrefix("min T = ");
    m_spinmin_T->setSuffix(" Days");

    // max_imum Time to Expiry (Days)
    m_spinmax_T = new QDoubleSpinBox(this);
    m_spinmax_T->setRange(minValue_T, maxValue_T);
    m_spinmax_T->setDecimals(2);
    m_spinmax_T->setSingleStep(1);
    m_spinmax_T->setValue(INIT_MAX_TIME_TO_EXPIRY);
    m_spinmax_T->setPrefix("max T = ");
    m_spinmax_T->setSuffix(" Days");

    // Time Layout
    m_layout_T = new QHBoxLayout();
    m_layout_T->addWidget(m_spinmin_T);
    m_layout_T->addWidget(m_spinmax_T);

    // Menu Layout
    m_leftLayout = new QVBoxLayout();
    m_leftLayout->addWidget(m_menuTitle);
    m_leftLayout->addWidget(m_button_STP);
    m_leftLayout->addWidget(m_button_SSP);
    m_leftLayout->addStretch();

    // Primary Layout
    m_rightLayout = new QVBoxLayout();
    m_rightLayout->addWidget(m_plot);
    m_rightLayout->addWidget(m_toggle_CP);
    m_rightLayout->addLayout(m_layout_S);
    m_rightLayout->addLayout(m_layout_K);
    m_rightLayout->addLayout(m_layout_r);
    m_rightLayout->addLayout(m_layout_sigma);
    m_rightLayout->addLayout(m_layout_T);

    // Full Layout
    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_leftLayout);
    m_mainLayout->addLayout(m_rightLayout);

    setLayout(m_mainLayout);
}

void Component::setupPlot() {

    m_plot = new QCustomPlot(this);
    m_plot->setMinimumHeight(300);

    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorScale = new QCPColorScale(m_plot);

    m_plot->plotLayout()->addElement(0, 1, m_colorScale);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->axis()->setLabel("Put Price");
    m_colorScale->axis()->setNumberFormat("f");
    m_colorScale->axis()->setNumberPrecision(2);
    m_colorMap->setColorScale(m_colorScale);
    m_colorMap->setGradient(QCPColorGradient::gpJet);
    m_colorMap->setInterpolate(true);
    m_colorMap->setTightBoundary(true);

    m_plot->plotLayout()->setColumnStretchFactor(0,4);
    m_plot->plotLayout()->setColumnStretchFactor(1,1);
}

int Component::spinToSliderLinear(double stepVal, double min, double max, int steps) {
    double ratio = (stepVal - min) / (max - min);
    return static_cast<int>(ratio * steps + 0.5); // +0.5 to fix off by 1 error
}

int Component::spinToSliderLog(double stepVal, double min, double max, int steps) {
    double logMin = std::log10(min);
    double logMax = std::log10(max);
    double logVal = std::log10(stepVal);
    double ratio = (logVal - logMin) / (logMax - logMin);
    return static_cast<int>(ratio * steps + 0.5); // +0.5 to fix off by 1 error
}

double Component::sliderToSpinLinear(int sliderVal, double min, double max, int steps) {
    double ratio = static_cast<double>(sliderVal) / steps;
    return min + ratio * (max - min);
}

double Component::sliderToSpinLog(int sliderVal, double min, double max, int steps) {
    double ratio = static_cast<double>(sliderVal) / steps;
    double logMin = std::log10(min);
    double logMax = std::log10(max);
    double logVal = logMin + ratio * (logMax - logMin);
    return std::pow(10.0, logVal);
}
