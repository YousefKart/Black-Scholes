#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>

#define INIT_YEAR 0
#define INIT_RISK 0.1
#define INIT_PRICE 100.0
#define INIT_MU 5
#define INIT_SIGMA 0.01
#define INIT_VOLATILITY 0.2

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("Black Scholes Model");
    w.resize(600,400);

    auto *spinT = new QDoubleSpinBox();
    spinT->setRange(0,1000);
    spinT->setValue(INIT_YEAR);
    spinT->setPrefix("T = ");

    auto *spinR = new QDoubleSpinBox();
    spinR->setRange(0,1);
    spinR->setValue(INIT_RISK);
    spinR->setPrefix("r = ");

    auto *spinS = new QDoubleSpinBox();
    spinS->setRange(0,1000000);
    spinS->setValue(INIT_PRICE);
    spinS->setPrefix("S = ");

    auto *spinMu = new QDoubleSpinBox();
    spinMu->setRange(0,1000000);
    spinMu->setValue(INIT_MU);
    spinMu->setPrefix(QString::fromUtf8(u8"\u03BC = "));

    auto *spinSigma = new QDoubleSpinBox();
    spinSigma->setRange(0,1);
    spinSigma->setValue(INIT_SIGMA);
    spinSigma->setPrefix(QString::fromUtf8(u8"\u03C3 = "));

    auto *spinA = new QDoubleSpinBox();
    spinA->setRange(0,1);
    spinA->setValue(INIT_VOLATILITY);
    spinA->setPrefix("a = ");

    auto *priceLabel = new QLabel(QString("Price: %1").arg(INIT_PRICE));

    auto *layout = new QVBoxLayout();
    layout->addWidget(spinS);
    layout->addWidget(spinR);
    layout->addWidget(spinMu);
    layout->addWidget(spinSigma);
    layout->addWidget(spinA);
    layout->addWidget(priceLabel);
    w.setLayout(layout);

    QObject::connect(spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                    [&](double S) {
                    priceLabel->setText(QString("Price: %1").arg(S));
    });

    w.show();
    return a.exec();
}
