#include "Compute.h"
#include "Functions.h"

Compute::Compute(Component& ui) :
    ui(ui),
    surfaceMode(Surface::SurfaceMode::STP),
    S(0), minS(0), maxS(0), currentS(0), deltaS(0),
    K(0), minK(0), maxK(0), currentK(0), deltaK(0),
    r(0), minr(0), maxr(0), currentr(0), deltar(0),
    q(0), minq(0), maxq(0), currentq(0), deltaq(0),
    sigma(0), minSigma(0), maxSigma(0), currentSigma(0), deltaSigma(0),
    T(0), minT(0), maxT(0), currentT(0), deltaT(0),
    d1(0), d2(0), Nd1(0), Nd2(0), C(0), P(0), price(0)
{
    // Recompute on update
    QObject::connect(ui.buttonGroup(), &QButtonGroup::idClicked, &ui, [this](int id) {surfaceMode = static_cast<Surface::SurfaceMode>(id); recompute();});
    QObject::connect(ui.toggleCP(), &QPushButton::toggled, [this]{recompute();});
    QObject::connect(ui.spinS(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinK(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinR(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinSigma(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinMinT(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});
    QObject::connect(ui.spinMaxT(), QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]{recompute();});

    recompute();
}

void Compute::recompute() {
    Option mode = ui.toggleCP()->isChecked() ? Option::PUT : Option::CALL;

    // Variables
    S = ui.spinS()->value();
    K = ui.spinK()->value();
    r = ui.spinR()->value() / 100.0; // Convert from % to 0.0-1.0 scale
    q = 0.0; // TODO: Add dividend yields
    sigma = ui.spinSigma()->value();
    minT = ui.spinMinT()->value() / 365.25; // Convert from days to years
    maxT = ui.spinMaxT()->value() / 365.25; // Convert from days to years
    T = maxT - minT;

    // Functions
    d1 = Functions::computeD1(S, K, r, q, sigma, T);
    d2 = Functions::computeD2(sigma, T, d1);
    Nd1 = Functions::computeN(d1);
    Nd2 = Functions::computeN(d2);
    C = Functions::computeC(S, K, r, q, T, d1, d2);
    P = Functions::computeP(S, K, r, q, T, d1, d2);

    // Change in Stock Price
    currentS = S, minS = 0.5*S, maxS = 1.5*S;
    deltaS = (maxS - minS) / (SAMPLES - 1);

    // Change in Strike Price
    currentK = K, minK = 0.5*K, maxK = 1.5*K;
    deltaK = (maxK - minK) / (SAMPLES - 1);

    // Change in Risk-Free Rate
    currentr = r, minr = 0.5*r, maxr = 1.5*r;
    deltar = (maxr - minr) / (SAMPLES - 1);

    // Change in Dividend Yields
    currentq = q, minq = 0.5*q, maxq = 1.5*q;
    deltaq = (maxq - minq) / (SAMPLES - 1);

    // Change in Volatility
    currentSigma = sigma, minSigma = 0.5*sigma, maxSigma = 1.5*sigma;
    deltaSigma = (maxSigma - minSigma) / (SAMPLES - 1);

    // Change in Time
    currentT = T;
    deltaT = (maxT - minT) / (SAMPLES - 1);

    QCPColorMapData *mapData = ui.colorMap()->data();
    mapData->setSize(SAMPLES, SAMPLES);
    mapData->setRange(QCPRange(minT, maxT), QCPRange(minS, maxS));

    auto computeOption = mode == Option::PUT ? Functions::computeP : Functions::computeC;

    for (int i = 0; i < SAMPLES; ++i) {
        switch (surfaceMode) {
        case Surface::SurfaceMode::STP:
            currentT += deltaT;
            break;
        case Surface::SurfaceMode::SSP:
            currentSigma += deltaSigma;
            break;
        default:
            break;
        }

        for (int j = 0; j < SAMPLES; ++j) {
            currentS = minS + j * deltaS;
            d1 = Functions::computeD1(currentS, currentK, currentr, currentq, currentSigma, currentT);
            d2 = Functions::computeD2(currentSigma, currentT, d1);

            price = computeOption(currentS, currentK, currentr, currentq, currentT, d1, d2);
            mapData->setCell(i, j, price);
        }
    }

    ui.toggleCP()->setText(mode == Option::PUT ? "Mode: Puts" : "Mode: Calls");
    ui.colorScale()->axis()->setLabel(mode == Option::PUT ? "Put Price" : "Call Price");
    ui.colorMap()->rescaleDataRange(true);
    ui.plot()->xAxis->setLabel(surfaceMode == Surface::SurfaceMode::STP ? "Years (T)" : "Volatility (\u03C3)");
    ui.plot()->yAxis->setLabel("Stock Price (S)");
    ui.plot()->xAxis->setRange(minT, maxT);
    ui.plot()->yAxis->setRange(minS, maxS);
    ui.plot()->replot();
}

