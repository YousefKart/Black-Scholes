#include <QApplication>
#include <cmath>
#include "component.h"
#include "compute.h"

#define SAMPLES 200

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyle("Fusion");

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30,30,30));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(20,20,20));
    darkPalette.setColor(QPalette::AlternateBase, QColor(30,30,30));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(40,40,40));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Highlight, QColor(0,200,255));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);

    app.setStyleSheet(R"(
        QPushButton {
            background-color: #2c2c2c;
            border: 1px solid #444;
            padding: 6px;
            border-radius: 4px;
        }

        QPushButton:checked {
            background-color: #222;
            border: 2px solid #00c8ff;
        }

        QPushButton:hover {
            border: 1px solid #00c8ff;
        }
    )");

    Component ui;
    ui.setWindowTitle("Black Scholes Model");
    ui.resize(750,450);

    Compute cpu(ui);

    ui.show();
    return app.exec();
}
