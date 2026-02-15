#include <QApplication>
#include <cmath>
#include "component.h"
#include "compute.h"

#define SAMPLES 200

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Component ui;
    ui.setWindowTitle("Black Scholes Model");
    ui.resize(700,450);

    Compute cpu(ui);

    ui.show();
    return app.exec();
}
