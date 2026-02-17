#include "component.h"

constexpr int WIDGET_WIDTH = 125;
constexpr int WIDGET_WIDTH_DOUBLE = WIDGET_WIDTH * 2 + 6; // Count 6 pixel gap
constexpr int MENU_WIDTH = 150;

constexpr double INIT_MIN_STOCK_PRICE = 100.0;
constexpr double INIT_MAX_STOCK_PRICE = 200.0;
constexpr double INIT_MIN_STRIKE_PRICE = 100.0;
constexpr double INIT_MAX_STRIKE_PRICE = 200.0;
constexpr double INIT_MIN_RISK_FREE_RATE = 5.0;
constexpr double INIT_MAX_RISK_FREE_RATE = 20.0;
constexpr double INIT_MIN_DIVIDEND_YIELD = 5.0;
constexpr double INIT_MAX_DIVIDEND_YIELD = 20.0;
constexpr double INIT_MIN_VOLATILITY = 0.2;
constexpr double INIT_MAX_VOLATILITY = 0.5;
constexpr double INIT_MIN_TIME_TO_EXPIRY = 0.00;
constexpr double INIT_MAX_TIME_TO_EXPIRY = 365.0;

Component::Component(QWidget* parent) : QWidget(parent)
{

    setupUI();
    setConfig(Surface::surfaceMap[Surface::SurfaceMode::STP]);

    this->setLayout(m_mainLayout);
}

void Component::setConfig(Surface::SurfaceConfig config) {
    this->config = config;

    m_slider_S->setVisible(config.input_S == Surface::InputType::SINGLE);
    m_spin_S->setVisible(config.input_S == Surface::InputType::SINGLE);
    m_rangeSlider_S->setVisible(config.input_S == Surface::InputType::RANGE);
    m_spinMin_S->setVisible(config.input_S == Surface::InputType::RANGE);
    m_spinMax_S->setVisible(config.input_S == Surface::InputType::RANGE);

    m_slider_K->setVisible(config.input_K == Surface::InputType::SINGLE);
    m_spin_K->setVisible(config.input_K == Surface::InputType::SINGLE);
    m_rangeSlider_K->setVisible(config.input_K == Surface::InputType::RANGE);
    m_spinMin_K->setVisible(config.input_K == Surface::InputType::RANGE);
    m_spinMax_K->setVisible(config.input_K == Surface::InputType::RANGE);

    m_slider_r->setVisible(config.input_r == Surface::InputType::SINGLE);
    m_spin_r->setVisible(config.input_r == Surface::InputType::SINGLE);
    m_rangeSlider_r->setVisible(config.input_r == Surface::InputType::RANGE);
    m_spinMin_r->setVisible(config.input_r == Surface::InputType::RANGE);
    m_spinMax_r->setVisible(config.input_r == Surface::InputType::RANGE);

    m_slider_q->setVisible(config.input_q == Surface::InputType::SINGLE);
    m_spin_q->setVisible(config.input_q == Surface::InputType::SINGLE);
    m_rangeSlider_q->setVisible(config.input_q == Surface::InputType::RANGE);
    m_spinMin_q->setVisible(config.input_q == Surface::InputType::RANGE);
    m_spinMax_q->setVisible(config.input_q == Surface::InputType::RANGE);

    m_slider_sigma->setVisible(config.input_sigma == Surface::InputType::SINGLE);
    m_spin_sigma->setVisible(config.input_sigma == Surface::InputType::SINGLE);
    m_rangeSlider_sigma->setVisible(config.input_sigma == Surface::InputType::RANGE);
    m_spinMin_sigma->setVisible(config.input_sigma == Surface::InputType::RANGE);
    m_spinMax_sigma->setVisible(config.input_sigma == Surface::InputType::RANGE);

    m_slider_T->setVisible(config.input_T == Surface::InputType::SINGLE);
    m_spin_T->setVisible(config.input_T == Surface::InputType::SINGLE);
    m_rangeSlider_T->setVisible(config.input_T == Surface::InputType::RANGE);
    m_spinMin_T->setVisible(config.input_T == Surface::InputType::RANGE);
    m_spinMax_T->setVisible(config.input_T == Surface::InputType::RANGE);

}

void Component::setupUI() {
    setupPlot();

    // Left-Hand Menu
    m_menuTitle = new QLabel("Surface Mode", this);
    m_menuTitle->setAlignment(Qt::AlignCenter);

    m_button_STP = new QPushButton("(S,T) -> Price", this);
    m_button_STP->setCheckable(true);
    m_button_STP->setMinimumWidth(MENU_WIDTH);
    m_button_STP->setMaximumWidth(MENU_WIDTH);
    m_button_STP->setChecked(true);

    m_button_SSP = new QPushButton(QString::fromUtf8(u8"(S,\u03C3) -> Price"), this);
    m_button_SSP->setCheckable(true);
    m_button_SSP->setMinimumWidth(MENU_WIDTH);
    m_button_SSP->setMaximumWidth(MENU_WIDTH);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    m_buttonGroup->addButton(m_button_STP, static_cast<int>(Surface::SurfaceMode::STP));
    m_buttonGroup->addButton(m_button_SSP, static_cast<int>(Surface::SurfaceMode::SIP));

    // Display Type (Calls/Puts)
    m_toggle_CP = new QPushButton(this);
    m_toggle_CP->setCheckable(true);

    // Stock Price
    m_label_S = new QLabel("S", this);

    m_slider_S = new QSlider(Qt::Horizontal, this);
    m_slider_S->setRange(0, SLIDER_RESOLUTION);
    m_slider_S->setValue(spinToSliderLog(INIT_MIN_STOCK_PRICE, minLimit_S, maxLimit_S, SLIDER_RESOLUTION));

    m_spin_S = new QDoubleSpinBox(this);
    m_spin_S->setRange(minLimit_S, maxLimit_S);
    m_spin_S->setDecimals(2);
    m_spin_S->setSingleStep(0.01);
    m_spin_S->setValue(INIT_MIN_STOCK_PRICE);
    m_spin_S->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_S->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_S = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_S->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_S->setLowerValue(spinToSliderLog(INIT_MIN_STOCK_PRICE, minLimit_S, maxLimit_S, SLIDER_RESOLUTION));
    m_rangeSlider_S->setUpperValue(spinToSliderLog(INIT_MAX_STOCK_PRICE, minLimit_S, maxLimit_S, SLIDER_RESOLUTION));

    qDebug() << "Slider raw:" << m_rangeSlider_S->lowerValue()
             << "Mapped value:"
             << sliderToSpinLog(m_rangeSlider_S->lowerValue(),
                                minLimit_S,
                                maxLimit_S,
                                SLIDER_RESOLUTION);

    qDebug() << "Slider raw:" << m_rangeSlider_S->upperValue()
             << "Mapped value:"
             << sliderToSpinLog(m_rangeSlider_S->upperValue(),
                                minLimit_S,
                                maxLimit_S,
                                SLIDER_RESOLUTION);



    m_spinMin_S = new QDoubleSpinBox(this);
    m_spinMin_S->setRange(minLimit_S, maxLimit_S);
    m_spinMin_S->setDecimals(2);
    m_spinMin_S->setSingleStep(0.01);
    m_spinMin_S->setValue(INIT_MIN_STOCK_PRICE);
    m_spinMin_S->setPrefix("Min: ");
    m_spinMin_S->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_S->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_S = new QDoubleSpinBox(this);
    m_spinMax_S->setRange(minLimit_S, maxLimit_S);
    m_spinMax_S->setDecimals(2);
    m_spinMax_S->setSingleStep(0.01);
    m_spinMax_S->setValue(INIT_MAX_STOCK_PRICE);
    m_spinMax_S->setPrefix("Max: ");
    m_spinMax_S->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_S->setMaximumWidth(WIDGET_WIDTH);

    m_layout_S = new QHBoxLayout();
    m_layout_S->addWidget(m_label_S);
    m_layout_S->addWidget(m_slider_S);
    m_layout_S->addWidget(m_spin_S);
    m_layout_S->addWidget(m_rangeSlider_S);
    m_layout_S->addWidget(m_spinMin_S);
    m_layout_S->addWidget(m_spinMax_S);

    // Strike Price
    m_label_K = new QLabel("K", this);

    m_slider_K = new QSlider(Qt::Horizontal, this);
    m_slider_K->setRange(0,SLIDER_RESOLUTION);
    m_slider_K->setValue(spinToSliderLog(INIT_MIN_STRIKE_PRICE, minLimit_K, maxLimit_K, SLIDER_RESOLUTION));

    m_spin_K = new QDoubleSpinBox(this);
    m_spin_K->setRange(minLimit_K, maxLimit_K);
    m_spin_K->setDecimals(2);
    m_spin_K->setSingleStep(0.01);
    m_spin_K->setValue(INIT_MIN_STRIKE_PRICE);
    m_spin_K->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_K->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_K = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_K->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_K->setLowerValue(spinToSliderLog(INIT_MIN_STRIKE_PRICE, minLimit_K, maxLimit_K, SLIDER_RESOLUTION));
    m_rangeSlider_K->setUpperValue(spinToSliderLog(INIT_MAX_STRIKE_PRICE, minLimit_K, maxLimit_K, SLIDER_RESOLUTION));

    m_spinMin_K = new QDoubleSpinBox(this);
    m_spinMin_K->setRange(minLimit_K, maxLimit_K);
    m_spinMin_K->setDecimals(2);
    m_spinMin_K->setSingleStep(0.01);
    m_spinMin_K->setValue(INIT_MIN_STRIKE_PRICE);
    m_spinMin_K->setPrefix("Min: ");
    m_spinMin_K->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_K->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_K = new QDoubleSpinBox(this);
    m_spinMax_K->setRange(minLimit_K, maxLimit_K);
    m_spinMax_K->setDecimals(2);
    m_spinMax_K->setSingleStep(0.01);
    m_spinMax_K->setValue(INIT_MAX_STRIKE_PRICE);
    m_spinMax_K->setPrefix("Max: ");
    m_spinMax_K->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_K->setMaximumWidth(WIDGET_WIDTH);

    m_layout_K = new QHBoxLayout();
    m_layout_K->addWidget(m_label_K);
    m_layout_K->addWidget(m_slider_K);
    m_layout_K->addWidget(m_spin_K);
    m_layout_K->addWidget(m_rangeSlider_K);
    m_layout_K->addWidget(m_spinMin_K);
    m_layout_K->addWidget(m_spinMax_K);

    // Risk-Free Rate (Annual)
    m_label_r = new QLabel("r", this);

    m_slider_r = new QSlider(Qt::Horizontal, this);
    m_slider_r->setRange(0, SLIDER_RESOLUTION);
    m_slider_r->setValue(spinToSliderLinear(INIT_MIN_RISK_FREE_RATE, minLimit_r, maxLimit_r, SLIDER_RESOLUTION));

    m_spin_r = new QDoubleSpinBox(this);
    m_spin_r->setRange(minLimit_r, maxLimit_r);
    m_spin_r->setDecimals(2);
    m_spin_r->setSingleStep(0.1);
    m_spin_r->setValue(INIT_MIN_RISK_FREE_RATE);
    m_spin_r->setSuffix("%");
    m_spin_r->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_r->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_r = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_r->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_r->setLowerValue(spinToSliderLinear(INIT_MIN_RISK_FREE_RATE, minLimit_r, maxLimit_r, SLIDER_RESOLUTION));
    m_rangeSlider_r->setUpperValue(spinToSliderLinear(INIT_MAX_RISK_FREE_RATE, minLimit_r, maxLimit_r, SLIDER_RESOLUTION));

    m_spinMin_r = new QDoubleSpinBox(this);
    m_spinMin_r->setRange(minLimit_r, maxLimit_r);
    m_spinMin_r->setDecimals(2);
    m_spinMin_r->setSingleStep(0.1);
    m_spinMin_r->setValue(INIT_MIN_RISK_FREE_RATE);
    m_spinMin_r->setPrefix("Min: ");
    m_spinMin_r->setSuffix("%");
    m_spinMin_r->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_r->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_r = new QDoubleSpinBox(this);
    m_spinMax_r->setRange(minLimit_r, maxLimit_r);
    m_spinMax_r->setDecimals(2);
    m_spinMax_r->setSingleStep(0.1);
    m_spinMax_r->setValue(INIT_MAX_RISK_FREE_RATE);
    m_spinMax_r->setPrefix("Max: ");
    m_spinMax_r->setSuffix("%");
    m_spinMax_r->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_r->setMaximumWidth(WIDGET_WIDTH);

    m_layout_r = new QHBoxLayout();
    m_layout_r->addWidget(m_label_r);
    m_layout_r->addWidget(m_slider_r);
    m_layout_r->addWidget(m_spin_r);
    m_layout_r->addWidget(m_rangeSlider_r);
    m_layout_r->addWidget(m_spinMin_r);
    m_layout_r->addWidget(m_spinMax_r);

    // Dividend Yield (Annual)
    m_label_q = new QLabel("q", this);

    m_slider_q = new QSlider(Qt::Horizontal, this);
    m_slider_q->setRange(0, SLIDER_RESOLUTION);
    m_slider_q->setValue(spinToSliderLinear(INIT_MAX_DIVIDEND_YIELD, minLimit_q, maxLimit_q, SLIDER_RESOLUTION));

    m_spin_q = new QDoubleSpinBox(this);
    m_spin_q->setRange(minLimit_q, maxLimit_q);
    m_spin_q->setDecimals(2);
    m_spin_q->setSingleStep(0.1);
    m_spin_q->setValue(INIT_MAX_DIVIDEND_YIELD);
    m_spin_q->setSuffix("%");
    m_spin_q->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_q->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_q = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_q->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_q->setLowerValue(spinToSliderLinear(INIT_MIN_DIVIDEND_YIELD, minLimit_q, maxLimit_q, SLIDER_RESOLUTION));
    m_rangeSlider_q->setUpperValue(spinToSliderLinear(INIT_MAX_DIVIDEND_YIELD, minLimit_q, maxLimit_q, SLIDER_RESOLUTION));

    m_spinMin_q = new QDoubleSpinBox(this);
    m_spinMin_q->setRange(minLimit_q, maxLimit_q);
    m_spinMin_q->setDecimals(2);
    m_spinMin_q->setSingleStep(0.1);
    m_spinMin_q->setValue(INIT_MIN_DIVIDEND_YIELD);
    m_spinMin_q->setPrefix("Min: ");
    m_spinMin_q->setSuffix("%");
    m_spinMin_q->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_q->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_q = new QDoubleSpinBox(this);
    m_spinMax_q->setRange(minLimit_q, maxLimit_q);
    m_spinMax_q->setDecimals(2);
    m_spinMax_q->setSingleStep(0.1);
    m_spinMax_q->setValue(INIT_MAX_DIVIDEND_YIELD);
    m_spinMax_q->setPrefix("Max: ");
    m_spinMax_q->setSuffix("%");
    m_spinMax_q->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_q->setMaximumWidth(WIDGET_WIDTH);

    m_layout_q = new QHBoxLayout();
    m_layout_q->addWidget(m_label_q);
    m_layout_q->addWidget(m_slider_q);
    m_layout_q->addWidget(m_spin_q);
    m_layout_q->addWidget(m_rangeSlider_q);
    m_layout_q->addWidget(m_spinMin_q);
    m_layout_q->addWidget(m_spinMax_q);

    // Volatility (Annual)
    m_label_sigma = new QLabel("\u03C3", this);

    m_slider_sigma = new QSlider(Qt::Horizontal, this);
    m_slider_sigma->setRange(0, SLIDER_RESOLUTION);
    m_slider_sigma->setValue(spinToSliderLinear(INIT_MIN_VOLATILITY, minLimit_sigma, maxLimit_sigma, SLIDER_RESOLUTION));

    m_spin_sigma = new QDoubleSpinBox(this);
    m_spin_sigma->setRange(minLimit_sigma, maxLimit_sigma);
    m_spin_sigma->setDecimals(4);
    m_spin_sigma->setSingleStep(0.01);
    m_spin_sigma->setValue(INIT_MIN_VOLATILITY);
    m_spin_sigma->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_sigma->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_sigma = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_sigma->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_sigma->setLowerValue(spinToSliderLinear(INIT_MIN_VOLATILITY, minLimit_sigma, maxLimit_sigma, SLIDER_RESOLUTION));
    m_rangeSlider_sigma->setUpperValue(spinToSliderLinear(INIT_MAX_VOLATILITY, minLimit_sigma, maxLimit_sigma, SLIDER_RESOLUTION));

    m_spinMin_sigma = new QDoubleSpinBox(this);
    m_spinMin_sigma->setRange(minLimit_sigma, maxLimit_sigma);
    m_spinMin_sigma->setDecimals(4);
    m_spinMin_sigma->setSingleStep(0.01);
    m_spinMin_sigma->setValue(INIT_MIN_VOLATILITY);
    m_spinMin_sigma->setPrefix("Min: ");
    m_spinMin_sigma->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_sigma->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_sigma = new QDoubleSpinBox(this);
    m_spinMax_sigma->setRange(minLimit_sigma, maxLimit_sigma);
    m_spinMax_sigma->setDecimals(4);
    m_spinMax_sigma->setSingleStep(0.01);
    m_spinMax_sigma->setValue(INIT_MAX_VOLATILITY);
    m_spinMax_sigma->setPrefix("Max: ");
    m_spinMax_sigma->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_sigma->setMaximumWidth(WIDGET_WIDTH);

    m_layout_sigma = new QHBoxLayout();
    m_layout_sigma->addWidget(m_label_sigma);
    m_layout_sigma->addWidget(m_slider_sigma);
    m_layout_sigma->addWidget(m_spin_sigma);
    m_layout_sigma->addWidget(m_rangeSlider_sigma);
    m_layout_sigma->addWidget(m_spinMin_sigma);
    m_layout_sigma->addWidget(m_spinMax_sigma);

    // Time
    m_label_T = new QLabel("T", this);

    m_slider_T = new QSlider(Qt::Horizontal, this);
    m_slider_T->setRange(0, SLIDER_RESOLUTION);
    m_slider_T->setValue(spinToSliderLinear(INIT_MIN_TIME_TO_EXPIRY, minLimit_T, maxLimit_T, SLIDER_RESOLUTION));

    m_spin_T = new QDoubleSpinBox(this);
    m_spin_T->setRange(minLimit_T, maxLimit_T);
    m_spin_T->setDecimals(2);
    m_spin_T->setSingleStep(1);
    m_spin_T->setValue(INIT_MIN_TIME_TO_EXPIRY);
    m_spin_T->setSuffix(" Days");
    m_spin_T->setMinimumWidth(WIDGET_WIDTH_DOUBLE);
    m_spin_T->setMaximumWidth(WIDGET_WIDTH_DOUBLE);

    m_rangeSlider_T = new RangeSlider(Qt::Horizontal, this);
    m_rangeSlider_T->setRange(0, SLIDER_RESOLUTION);
    m_rangeSlider_T->setLowerValue(spinToSliderLinear(INIT_MIN_TIME_TO_EXPIRY, minLimit_T, maxLimit_T, SLIDER_RESOLUTION));
    m_rangeSlider_T->setUpperValue(spinToSliderLinear(INIT_MAX_TIME_TO_EXPIRY, minLimit_T, maxLimit_T, SLIDER_RESOLUTION));

    m_spinMin_T = new QDoubleSpinBox(this);
    m_spinMin_T->setRange(minLimit_T, maxLimit_T);
    m_spinMin_T->setDecimals(2);
    m_spinMin_T->setSingleStep(1);
    m_spinMin_T->setValue(INIT_MIN_TIME_TO_EXPIRY);
    m_spinMin_T->setPrefix("Min: ");
    m_spinMin_T->setSuffix(" Days");
    m_spinMin_T->setMinimumWidth(WIDGET_WIDTH);
    m_spinMin_T->setMaximumWidth(WIDGET_WIDTH);

    m_spinMax_T = new QDoubleSpinBox(this);
    m_spinMax_T->setRange(minLimit_T, maxLimit_T);
    m_spinMax_T->setDecimals(2);
    m_spinMax_T->setSingleStep(1);
    m_spinMax_T->setValue(INIT_MAX_TIME_TO_EXPIRY);
    m_spinMax_T->setPrefix("Max: ");
    m_spinMax_T->setSuffix(" Days");
    m_spinMax_T->setMinimumWidth(WIDGET_WIDTH);
    m_spinMax_T->setMaximumWidth(WIDGET_WIDTH);

    m_layout_T = new QHBoxLayout();
    m_layout_T->addWidget(m_label_T);
    m_layout_T->addWidget(m_slider_T);
    m_layout_T->addWidget(m_spin_T);
    m_layout_T->addWidget(m_rangeSlider_T);
    m_layout_T->addWidget(m_spinMin_T);
    m_layout_T->addWidget(m_spinMax_T);

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
    m_rightLayout->addLayout(m_layout_q);
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
