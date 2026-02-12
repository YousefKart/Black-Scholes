#include <QApplication>
#include <cmath>
#include "Component.h"
#include "Compute.h"

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
