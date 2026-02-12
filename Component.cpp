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
    m_menuTitle = new QLabel("Select Surface Mode", this);

    m_buttonSTP = new QPushButton("(S,T) -> Price", this);
    m_buttonSTP->setCheckable(true);
    m_buttonSTP->setMinimumWidth(150);
    m_buttonSTP->setMaximumWidth(150);
    m_buttonSTP->setChecked(true);

    m_buttonSSP = new QPushButton(QString::fromUtf8(u8"(S,\u03C3) -> Price"), this);
    m_buttonSSP->setCheckable(true);
    m_buttonSSP->setMinimumWidth(150);
    m_buttonSSP->setMaximumWidth(150);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    m_buttonGroup->addButton(m_buttonSTP, static_cast<int>(Surface::SurfaceMode::STP));
    m_buttonGroup->addButton(m_buttonSSP, static_cast<int>(Surface::SurfaceMode::SSP));

    // Display Type (Calls/Puts)
    m_toggleCP = new QPushButton(this);
    m_toggleCP->setCheckable(true);

    // Stock Price
    m_spinS = new QDoubleSpinBox(this);
    m_spinS->setRange(0.01,1000000.0);
    m_spinS->setValue(INIT_STOCK_PRICE);
    m_spinS->setPrefix("S = ");

    // Strike Price
    m_spinK = new QDoubleSpinBox(this);
    m_spinK->setRange(0.01,1000000.0);
    m_spinK->setValue(INIT_STRIKE_PRICE);
    m_spinK->setPrefix("K = ");

    // Risk-Free Rate (Annual)
    m_spinR = new QDoubleSpinBox(this);
    m_spinR->setRange(0.0,100.0);
    m_spinR->setDecimals(2);
    m_spinR->setSingleStep(0.1);
    m_spinR->setValue(INIT_RISK_FREE_RATE);
    m_spinR->setPrefix("r = ");
    m_spinR->setSuffix(" %");

    // Volatility (Annual)
    m_spinSigma = new QDoubleSpinBox(this);
    m_spinSigma->setRange(0.0001,5.0);
    m_spinSigma->setDecimals(4);
    m_spinSigma->setSingleStep(0.01);
    m_spinSigma->setValue(INIT_VOLATILITY);
    m_spinSigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));

    // Mininum Time to Expiry (Days)
    m_spinMinT = new QDoubleSpinBox(this);
    m_spinMinT->setRange(0.0001,10000.0);
    m_spinMinT->setDecimals(2);
    m_spinMinT->setSingleStep(1);
    m_spinMinT->setValue(INIT_MIN_TIME_TO_EXPIRY);
    m_spinMinT->setPrefix("Min T = ");
    m_spinMinT->setSuffix(" Days");

    // Maximum Time to Expiry (Days)
    m_spinMaxT = new QDoubleSpinBox(this);
    m_spinMaxT->setRange(0.0001,10000.0);
    m_spinMaxT->setDecimals(2);
    m_spinMaxT->setSingleStep(1);
    m_spinMaxT->setValue(INIT_MAX_TIME_TO_EXPIRY);
    m_spinMaxT->setPrefix("Max T = ");
    m_spinMaxT->setSuffix(" Days");

    // Time Layout
    m_rangeLayoutT = new QHBoxLayout();
    m_rangeLayoutT->addWidget(m_spinMinT);
    m_rangeLayoutT->addWidget(m_spinMaxT);

    // Menu Layout
    m_leftLayout = new QVBoxLayout();
    m_leftLayout->addWidget(m_menuTitle);
    m_leftLayout->addWidget(m_buttonSTP);
    m_leftLayout->addWidget(m_buttonSSP);
    m_leftLayout->addStretch();

    // Primary Layout
    m_rightLayout = new QVBoxLayout();
    m_rightLayout->addWidget(m_plot);
    m_rightLayout->addWidget(m_toggleCP);
    m_rightLayout->addWidget(m_spinS);
    m_rightLayout->addWidget(m_spinK);
    m_rightLayout->addWidget(m_spinR);
    m_rightLayout->addWidget(m_spinSigma);
    m_rightLayout->addLayout(m_rangeLayoutT);

    // Full Layout
    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_leftLayout);
    m_mainLayout->addLayout(m_rightLayout);
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
