#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <cmath>
#include "qcustomplot.h"

#define INIT_STOCK_PRICE 100.0
#define INIT_STRIKE_PRICE 110.0
#define INIT_RISK_FREE_RATE 10.0
#define INIT_VOLATILITY 0.2
#define INIT_MIN_TIME_TO_EXPIRY 0.00
#define INIT_MAX_TIME_TO_EXPIRY 365.0

class Component
{

public:
    Component();

    // Window
    QWidget window;

    // Menu
    QLabel *menuTitle;
    QPushButton *buttonSTP;
    QPushButton *buttonSSP;
    QButtonGroup *buttonGroup;

    // Plot
    QCustomPlot *plot;

    // Color Map & Legend
    QCPColorMap *colorMap;
    QCPColorScale *colorScale;

    // Price Display Switch (Calls/Puts)
    QPushButton *pushToggleCP;

    // S Components
    QDoubleSpinBox *spinS; // Stock Price

    // K Components
    QDoubleSpinBox *spinK; // Strike Price

    // r Components
    QDoubleSpinBox *spinR; // Risk-Free Rate (Annual)

    // σ Components
    QDoubleSpinBox *spinSigma; // Volatility (Annual)

    // T Components
    QDoubleSpinBox *spinMinT; // Mininum Time to Expiry (Days)
    QDoubleSpinBox *spinMaxT; // Maximum Time to Expiry (Days)
    QHBoxLayout *rangeLayoutT;  // Horizontal Layout

    // Main Layout
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *mainLayout;
};

#endif // COMPONENT_H
