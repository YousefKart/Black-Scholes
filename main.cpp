#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <cmath>
#include "qcustomplot.h"
#include "Functions.h"

#define INIT_STOCK_PRICE 100.0
#define INIT_STRIKE_PRICE 110.0
#define INIT_RISK_FREE_RATE 10.0
#define INIT_VOLATILITY 0.2
#define INIT_MIN_TIME_TO_EXPIRY 0.00
#define INIT_MAX_TIME_TO_EXPIRY 365.0

#define SAMPLES 200

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("Black Scholes Model");
    w.resize(600,400);

    // Plot
    auto *plot = new QCustomPlot();
    plot->addGraph();
    plot->xAxis->setLabel("Years (T)");
    plot->yAxis->setLabel("Stock Price (S)");
    plot->setMinimumHeight(300);

    // Color Map & Legend
    auto *colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    auto *colorScale = new QCPColorScale(plot);
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
    auto *pushToggleCP = new QPushButton();
    pushToggleCP->setCheckable(true);

    // Stock Price
    auto *spinS = new QDoubleSpinBox();
    spinS->setRange(0.01,1000000.0);
    spinS->setValue(INIT_STOCK_PRICE);
    spinS->setPrefix("S = ");

    // Strike Price
    auto *spinK = new QDoubleSpinBox();
    spinK->setRange(0.01,1000000.0);
    spinK->setValue(INIT_STRIKE_PRICE);
    spinK->setPrefix("K = ");

    // Risk-Free Rate (Annual)
    auto *spinR = new QDoubleSpinBox();
    spinR->setRange(0.0,100.0);
    spinR->setDecimals(2);
    spinR->setSingleStep(0.1);
    spinR->setValue(INIT_RISK_FREE_RATE);
    spinR->setPrefix("r = ");
    spinR->setSuffix(" %");

    // Volatility (Annual)
    auto *spinSigma = new QDoubleSpinBox();
    spinSigma->setRange(0.0001,5.0);
    spinSigma->setDecimals(4);
    spinSigma->setSingleStep(0.01);
    spinSigma->setValue(INIT_VOLATILITY);
    spinSigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));

    // Mininum Time to Expiry (Days)
    auto *spinMinT = new QDoubleSpinBox();
    spinMinT->setRange(0.0001,10000.0);
    spinMinT->setDecimals(2);
    spinMinT->setSingleStep(1);
    spinMinT->setValue(INIT_MIN_TIME_TO_EXPIRY);
    spinMinT->setPrefix("Min T = ");
    spinMinT->setSuffix(" Days");

    // Maximum Time to Expiry (Days)
    auto *spinMaxT = new QDoubleSpinBox();
    spinMaxT->setRange(0.0001,10000.0);
    spinMaxT->setDecimals(2);
    spinMaxT->setSingleStep(1);
    spinMaxT->setValue(INIT_MAX_TIME_TO_EXPIRY);
    spinMaxT->setPrefix("Max T = ");
    spinMaxT->setSuffix(" Days");

    // Horizontal Layout
    auto *rangeLayoutT = new QHBoxLayout();
    rangeLayoutT->addWidget(spinMinT);
    rangeLayoutT->addWidget(spinMaxT);

    auto recompute = [&]() {
        auto S = spinS->value();
        auto K = spinK->value();
        auto r = spinR->value() / 100.0; // Convert from % to 0.0-1.0 scale
        auto q = 0.0; // TODO: Add dividend yields
        auto sigma = spinSigma->value();
        auto minT = spinMinT->value() / 365.25; // Convert from days to years
        auto maxT = spinMaxT->value() / 365.25; // Convert from days to years
        auto T = maxT - minT;

        auto d1 = Functions::computeD1(S, K, r, q, sigma, T);
        auto d2 = Functions::computeD2(sigma, T, d1);
        auto Nd1 = Functions::computeN(d1);
        auto Nd2 = Functions::computeN(d2);
        auto C = Functions::computeC(S, K, r, q, T, d1, d2);
        auto P = Functions::computeP(S, K, r, q, T, d1, d2);

        double time, sample, price;
        double minS = 0.5*K, maxS = 1.5*K;
        double deltaT = (maxT - minT) / (SAMPLES - 1);
        double deltaS = (maxS - minS) / (SAMPLES - 1);

        colorMap->data()->setSize(SAMPLES, SAMPLES);
        colorMap->data()->setRange(QCPRange(minT, maxT), QCPRange(minS, maxS));
        for (int i = 0; i < SAMPLES; ++i) {
            time = minT + i * deltaT;
            for (int j = 0; j < SAMPLES; ++j) {
                sample = minS + j * deltaS;
                d1 = Functions::computeD1(sample, K, r, q, sigma, time);
                d2 = Functions::computeD2(sigma, time, d1);
                C = Functions::computeC(sample, K, r, q, time, d1, d2);
                P = Functions::computeP(sample, K, r, q, time, d1, d2);

                price = pushToggleCP->isChecked() ? P : C;
                colorMap->data()->setCell(i, j, price);
            }
        }

        pushToggleCP->setText(pushToggleCP->isChecked() ? "Mode: Puts" : "Mode: Calls");
        colorScale->axis()->setLabel(pushToggleCP->isChecked() ? "Put Price" : "Call Price");
        colorMap->rescaleDataRange(true);
        plot->xAxis->setRange(minT, maxT);
        plot->yAxis->setRange(minS, maxS);
        plot->replot();
    };

    // Recompute on update
    QObject::connect(pushToggleCP, &QPushButton::toggled, [&]{recompute();});
    QObject::connect(spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinK, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinMinT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinMaxT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});

    // Layout
    auto *layout = new QVBoxLayout();
    layout->addWidget(plot);
    layout->addWidget(pushToggleCP);
    layout->addWidget(spinS);
    layout->addWidget(spinK);
    layout->addWidget(spinR);
    layout->addWidget(spinSigma);
    layout->addLayout(rangeLayoutT);

    w.setLayout(layout);
    recompute();
    w.show();

    return a.exec();
}
