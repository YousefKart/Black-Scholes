#ifndef SURFACE_H
#define SURFACE_H

#include <QObject>
#include <QMetaEnum>
#include <QString>

class Surface : public QObject
{
    Q_OBJECT

public:
    enum class SurfaceMode {
        STP, // (S,T) -> Price
        SSP, // (S,σ) -> Price
        SMD  // (S,σ) -> Delta
    };

    Q_ENUM(SurfaceMode);
};

#endif // SURFACE_H
