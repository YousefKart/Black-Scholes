#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>

#define INIT_PRICE 100.0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("Black Scholes Model");

    auto *spinS = new QDoubleSpinBox();
    spinS->setRange(0,1000000);
    spinS->setValue(INIT_PRICE);
    spinS->setPrefix("S = ");

    auto *priceLabel = new QLabel(QString("Price: %1").arg(INIT_PRICE));

    auto *layout = new QVBoxLayout();
    layout->addWidget(spinS);
    layout->addWidget(priceLabel);
    w.setLayout(layout);

    QObject::connect(spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                    [&](double S) {
                    priceLabel->setText(QString("Price: %1").arg(S));
    });

    w.show();
    return a.exec();
}
