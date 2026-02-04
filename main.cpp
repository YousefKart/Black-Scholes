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
#define INIT_TIME_TO_EXPIRY 365.0


int main(int argc, char *argv[])
{
    const int SAMPLES = 200;
    QVector<double> plotX(SAMPLES), plotY0(SAMPLES), plotY1(SAMPLES);

    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("Black Scholes Model");
    w.resize(600,400);

    // Plot
    auto *plot = new QCustomPlot();
    plot->addGraph(); // Graph 0 for calls
    plot->addGraph(); // Graph 1 for puts
    plot->xAxis->setLabel("Stock Price (S)");
    plot->yAxis->setLabel("Option Price");
    plot->setMinimumHeight(300);

    // Plot Pens
    QPen pen0, pen1;
    pen0.setColor(Qt::blue);
    pen1.setColor(Qt::red);
    plot->graph(0)->setPen(pen0);
    plot->graph(1)->setPen(pen1);

    // Plot Legend
    plot->legend->setVisible(true);
    plot->legend->setBrush(QColor(255,255,255,100));
    plot->graph(0)->setName("Calls");
    plot->graph(1)->setName("Puts");

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
    spinR->setDecimals(4);
    spinR->setSingleStep(0.1);
    spinR->setValue(INIT_RISK_FREE_RATE);
    spinR->setPrefix("r = ");
    spinR->setSuffix("%");

    // Volatility (Annual)
    auto *spinSigma = new QDoubleSpinBox();
    spinSigma->setRange(0.0001,5.0);
    spinSigma->setDecimals(4);
    spinSigma->setSingleStep(0.01);
    spinSigma->setValue(INIT_VOLATILITY);
    spinSigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));

    // Time to Expiry (Days)
    auto *spinT = new QDoubleSpinBox();
    spinT->setRange(0.0001,10000.0);
    spinT->setDecimals(2);
    spinT->setSingleStep(1);
    spinT->setValue(INIT_TIME_TO_EXPIRY);
    spinT->setPrefix("T = ");
    spinT->setSuffix(" Days");

    auto *d1Label = new QLabel(QString("d1: --"));
    auto *d2Label = new QLabel(QString("d2: --"));
    auto *Nd1Label = new QLabel(QString("N(d1): --"));
    auto *Nd2Label = new QLabel(QString("N(d2): --"));
    auto *CLabel = new QLabel(QString("Call: --"));
    auto *PLabel = new QLabel(QString("Put: --"));

    auto recompute = [&]() {
        auto S = spinS->value();
        auto K = spinK->value();
        auto r = spinR->value() / 100.0; // Convert from % to 0.0-1.0 scale
        auto q = 0.0; // TODO: Add dividend yields
        auto sigma = spinSigma->value();
        auto T = spinT->value() / 365.25; // Convert from days to years

        auto d1 = Functions::computeD1(S, K, r, q, sigma, T);
        auto d2 = Functions::computeD2(sigma, T, d1);
        auto Nd1 = Functions::computeN(d1);
        auto Nd2 = Functions::computeN(d2);
        auto C = Functions::computeC(S, K, r, q, T, d1, d2);
        auto P = Functions::computeP(S, K, r, q, T, d1, d2);

        d1Label->setText(QString("d1: %1").arg(d1));
        d2Label->setText(QString("d2: %1").arg(d2));
        Nd1Label->setText(QString("N(d1): %1").arg(Nd1));
        Nd2Label->setText(QString("N(d2): %1").arg(Nd2));
        CLabel->setText(QString("Call: %1").arg(C));
        PLabel->setText(QString("Put: %1").arg(P));

        double minS = 0.5*K;
        double maxS = 1.5*K;
        double deltaS = (maxS - minS) / (SAMPLES - 1);

        for (int i = 0; i < SAMPLES; i++) {
            double sample = minS + i * deltaS;
            double d1 = Functions::computeD1(sample, K, r, q, sigma, T);
            double d2 = Functions::computeD2(sigma, T, d1);
            double C = Functions::computeC(sample, K, r, q, T, d1, d2);
            double P = Functions::computeP(S, K, r, q, T, d1, d2);

            plotX[i] = sample;
            plotY0[i] = C;
            plotY1[i] = P;
        }

        plot->graph(0)->setData(plotX, plotY0);
        plot->graph(1)->setData(plotX, plotY1);
        plot->rescaleAxes();
        plot->yAxis->setPadding(10);
        plot->replot();
    };

    // Recompute on update
    QObject::connect(spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinK, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});

    // Layout
    auto *layout = new QVBoxLayout();
    layout->addWidget(plot);
    layout->addWidget(spinS);
    layout->addWidget(spinK);
    layout->addWidget(spinR);
    layout->addWidget(spinSigma);
    layout->addWidget(spinT);

    // layout->addWidget(d1Label);
    // layout->addWidget(d2Label);
    // layout->addWidget(Nd1Label);
    // layout->addWidget(Nd2Label);
    // layout->addWidget(CLabel);
    // layout->addWidget(PLabel);

    w.setLayout(layout);
    recompute();
    w.show();

    return a.exec();
}
