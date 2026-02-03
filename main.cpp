#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <cmath>

#define INIT_STOCK_PRICE 100.0
#define INIT_STRIKE_PRICE 100.0
#define INIT_RISK_FREE_RATE 0.1
#define INIT_VOLATILITY 0.2
#define INIT_TIME_TO_EXPIRY 1.0


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("Black Scholes Model");
    w.resize(600,400);

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
    spinR->setRange(0.0,1.0);
    spinR->setDecimals(4);
    spinR->setSingleStep(0.01);
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

    // Time to Expiry (Years)
    auto *spinT = new QDoubleSpinBox();
    spinT->setRange(0.0001,1000.0);
    spinT->setDecimals(6);
    spinT->setSingleStep(0.1);
    spinT->setValue(INIT_TIME_TO_EXPIRY);
    spinT->setPrefix("T = ");
    spinT->setSuffix(" Years");

    auto *d1Label = new QLabel(QString("d1: --"));
    auto *d2Label = new QLabel(QString("d2: --"));

    auto recompute = [&]() {
        auto S = spinS->value();
        auto K = spinK->value();
        auto r = spinR->value();
        auto sigma = spinSigma->value();
        auto T = spinT->value();

        auto d1 = (std::log(S/K) + (r + sigma*sigma*0.5) * T) / (sigma*std::sqrt(T));
        auto d2 = d1 - sigma*std::sqrt(T);

        d1Label->setText(QString("d1: %1").arg(d1));
        d2Label->setText(QString("d2: %1").arg(d2));
    };

    // Recompute on variable update
    QObject::connect(spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinK, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(spinT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});

    // Layout
    auto *layout = new QVBoxLayout();
    layout->addWidget(spinS);
    layout->addWidget(spinK);
    layout->addWidget(spinR);
    layout->addWidget(spinSigma);
    layout->addWidget(spinT);
    layout->addWidget(d1Label);
    layout->addWidget(d2Label);
    w.setLayout(layout);

    recompute();
    w.show();
    return a.exec();
}
