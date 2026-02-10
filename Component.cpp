#include "Component.h"
#include <QMetaEnum>
#include "Surface.h"

Component::Component()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Surface::SurfaceMode>();

    // Menu Title
    menuTitle = new QLabel();
    menuTitle->setText("Select Surface Mode");

    buttonSTP = new QPushButton("(S,T) -> Price", &window);
    buttonSTP->setCheckable(true);
    buttonSTP->setMinimumWidth(150);
    buttonSTP->setMaximumWidth(150);
    buttonSTP->setChecked(true);

    buttonSSP = new QPushButton(QString::fromUtf8(u8"(S,\u03C3) -> Price"), &window);
    buttonSSP->setCheckable(true);
    buttonSSP->setMinimumWidth(150);
    buttonSSP->setMaximumWidth(150);

    buttonGroup = new QButtonGroup(&window);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(buttonSTP, static_cast<int>(Surface::SurfaceMode::STP));
    buttonGroup->addButton(buttonSSP, static_cast<int>(Surface::SurfaceMode::SSP));

    // Plot
    plot = new QCustomPlot(&window);
    plot->addGraph();
    plot->setMinimumHeight(300);

    // Color Map & Legend
    colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    colorScale = new QCPColorScale(plot);

    plot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorScale->axis()->setLabel("Put Price");
    colorScale->axis()->setNumberFormat("f");
    colorScale->axis()->setNumberPrecision(2);
    colorMap->setColorScale(colorScale);
    colorMap->setGradient(QCPColorGradient::gpJet);
    colorMap->setInterpolate(true);
    colorMap->setTightBoundary(true);

    plot->plotLayout()->setColumnStretchFactor(0,4);
    plot->plotLayout()->setColumnStretchFactor(1,1);

    // Display Type (Calls/Puts)
    pushToggleCP = new QPushButton(&window);
    pushToggleCP->setCheckable(true);

    // Stock Price
    spinS = new QDoubleSpinBox(&window);
    spinS->setRange(0.01,1000000.0);
    spinS->setValue(INIT_STOCK_PRICE);
    spinS->setPrefix("S = ");

    // Strike Price
    spinK = new QDoubleSpinBox(&window);
    spinK->setRange(0.01,1000000.0);
    spinK->setValue(INIT_STRIKE_PRICE);
    spinK->setPrefix("K = ");

    // Risk-Free Rate (Annual)
    spinR = new QDoubleSpinBox(&window);
    spinR->setRange(0.0,100.0);
    spinR->setDecimals(2);
    spinR->setSingleStep(0.1);
    spinR->setValue(INIT_RISK_FREE_RATE);
    spinR->setPrefix("r = ");
    spinR->setSuffix(" %");

    // Volatility (Annual)
    spinSigma = new QDoubleSpinBox(&window);
    spinSigma->setRange(0.0001,5.0);
    spinSigma->setDecimals(4);
    spinSigma->setSingleStep(0.01);
    spinSigma->setValue(INIT_VOLATILITY);
    spinSigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));

    // Mininum Time to Expiry (Days)
    spinMinT = new QDoubleSpinBox(&window);
    spinMinT->setRange(0.0001,10000.0);
    spinMinT->setDecimals(2);
    spinMinT->setSingleStep(1);
    spinMinT->setValue(INIT_MIN_TIME_TO_EXPIRY);
    spinMinT->setPrefix("Min T = ");
    spinMinT->setSuffix(" Days");

    // Maximum Time to Expiry (Days)
    spinMaxT = new QDoubleSpinBox(&window);
    spinMaxT->setRange(0.0001,10000.0);
    spinMaxT->setDecimals(2);
    spinMaxT->setSingleStep(1);
    spinMaxT->setValue(INIT_MAX_TIME_TO_EXPIRY);
    spinMaxT->setPrefix("Max T = ");
    spinMaxT->setSuffix(" Days");

    // Time Layout
    rangeLayoutT = new QHBoxLayout();
    rangeLayoutT->addWidget(spinMinT);
    rangeLayoutT->addWidget(spinMaxT);

    // Menu Layout
    leftLayout = new QVBoxLayout();
    leftLayout->addWidget(menuTitle);
    leftLayout->addWidget(buttonSTP);
    leftLayout->addWidget(buttonSSP);
    leftLayout->addStretch();

    // Primary Layout
    rightLayout = new QVBoxLayout();
    rightLayout->addWidget(plot);
    rightLayout->addWidget(pushToggleCP);
    rightLayout->addWidget(spinS);
    rightLayout->addWidget(spinK);
    rightLayout->addWidget(spinR);
    rightLayout->addWidget(spinSigma);
    rightLayout->addLayout(rangeLayoutT);

    // Full Layout
    mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    window.setLayout(mainLayout);
}
