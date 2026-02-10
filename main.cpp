#include <QApplication>
#include <cmath>
#include "Component.h"
#include "Functions.h"
#include "Surface.h"

#define SAMPLES 200

int main(int argc, char *argv[])
{
    Surface::SurfaceMode surfaceMode = Surface::SurfaceMode::STP;

    QApplication app(argc, argv);
    Component ui;

    ui.window.setWindowTitle("Black Scholes Model");
    ui.window.resize(600,400);

    auto recompute = [&]() {
        bool putsMode = ui.pushToggleCP->isChecked();
        bool callsMode = !putsMode; // May be unused

        auto S = ui.spinS->value();
        auto K = ui.spinK->value();
        auto r = ui.spinR->value() / 100.0; // Convert from % to 0.0-1.0 scale
        auto q = 0.0; // TODO: Add dividend yields
        auto sigma = ui.spinSigma->value();
        auto minT = ui.spinMinT->value() / 365.25; // Convert from days to years
        auto maxT = ui.spinMaxT->value() / 365.25; // Convert from days to years
        auto T = maxT - minT;

        // Functions
        auto d1 = Functions::computeD1(S, K, r, q, sigma, T);
        auto d2 = Functions::computeD2(sigma, T, d1);
        auto Nd1 = Functions::computeN(d1);
        auto Nd2 = Functions::computeN(d2);
        auto C = Functions::computeC(S, K, r, q, T, d1, d2);
        auto P = Functions::computeP(S, K, r, q, T, d1, d2);

        // Min-Max values with steps (deltas)
        double tempS = S, minS = 0.5*K, maxS = 1.5*K;
        double deltaS = (maxS - minS) / (SAMPLES - 1);

        double tempSigma = sigma, minSigma = 0.5*sigma, maxSigma = 1.5*sigma;
        double deltaSigma = (maxSigma - minSigma) / (SAMPLES - 1);

        double tempT = T;
        double deltaT = (maxT - minT) / (SAMPLES - 1);

        double price;

        ui.colorMap->data()->setSize(SAMPLES, SAMPLES);
        ui.colorMap->data()->setRange(QCPRange(minT, maxT), QCPRange(minS, maxS));

        for (int i = 0; i < SAMPLES; ++i) {
            switch (surfaceMode) {
            case Surface::SurfaceMode::STP:
                tempT = minT + i * deltaT;
                break;
            case Surface::SurfaceMode::SSP:
                tempSigma = minSigma + i * deltaSigma;
                break;
            default:
                break;
            }

            for (int j = 0; j < SAMPLES; ++j) {
                tempS = minS + j * deltaS;
                d1 = Functions::computeD1(tempS, K, r, q, tempSigma, tempT);
                d2 = Functions::computeD2(tempSigma, tempT, d1);
                C = Functions::computeC(tempS, K, r, q, tempT, d1, d2);
                P = Functions::computeP(tempS, K, r, q, tempT, d1, d2);

                price = putsMode ? P : C;
                ui.colorMap->data()->setCell(i, j, price);
            }
        }

        ui.pushToggleCP->setText(putsMode ? "Mode: Puts" : "Mode: Calls");
        ui.colorScale->axis()->setLabel(putsMode ? "Put Price" : "Call Price");
        ui.colorMap->rescaleDataRange(true);
        ui.plot->xAxis->setLabel(surfaceMode == Surface::SurfaceMode::STP ? "Years (T)" : "Volatility (\u03C3)");
        ui.plot->yAxis->setLabel(surfaceMode == Surface::SurfaceMode::SMD ? "Delta" : "Stock Price (S)");
        ui.plot->xAxis->setRange(minT, maxT);
        ui.plot->yAxis->setRange(minS, maxS);
        ui.plot->replot();
    };

    // Recompute on update
    QObject::connect(ui.buttonGroup, &QButtonGroup::idClicked, &ui.window, [&](int id) {surfaceMode = static_cast<Surface::SurfaceMode>(id); recompute();});
    QObject::connect(ui.pushToggleCP, &QPushButton::toggled, [&]{recompute();});
    QObject::connect(ui.spinS, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(ui.spinK, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(ui.spinR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(ui.spinSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(ui.spinMinT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});
    QObject::connect(ui.spinMaxT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{recompute();});

    // Run
    recompute();
    ui.window.show();

    return app.exec();
}
