#ifndef SURFACE_H
#define SURFACE_H

#include <QObject>
#include <QString>

class Surface : public QObject
{
    Q_OBJECT

public:
    enum class SurfaceMode {
        STP, // (S,T) -> Price
        SSP, // (S,σ) -> Price
        SSD  // (S,σ) -> Delta
    };

    enum class InputType {
        NONE, // Do not show
        SINGLE, // Show constant value
        RANGE // Show range between two values
    };

    struct Config {
        double xValue;
        double yValue;
        double zValue;
        QString xLabel;
        QString yLabel;
        QString zLabel;
        InputType sInput = InputType::NONE;
        InputType kInput = InputType::NONE;
        InputType rInput = InputType::NONE;
        InputType qInput = InputType::NONE;
        InputType sigmaInput = InputType::NONE;
        InputType tInput = InputType::NONE;
    };
};

#endif // SURFACE_H
