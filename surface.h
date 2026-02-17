#ifndef SURFACE_H
#define SURFACE_H

#include <QObject>
#include <QString>

class Surface : public QObject
{
    Q_OBJECT

public:
    Surface();

    enum class SurfaceMode {
        /*
         * S = Stock Price
         * K = Strike Price
         * R = Risk-Free Return
         * Q = Dividends
         * I = Volatility
         * T = Time
         * ->
         * P = Price
         * D = Delta
         * G = Gamma
         * V = Vega
         * H = Theta
         * O = Rho
         * M = Implied Volatility
         * */

        SKP, // (S,K) -> Price
        SRP, // (S,r) -> Price
        SIP, // (S,σ) -> Price
        STP, // (S,T) -> Price
        KRP, // (K,r) -> Price
        KIP, // (K,σ) -> Price
        KTP, // (K,T) -> Price
        RTP, // (r,T) -> Price
        ITP, // (σ,T) -> Price

        SKD, // (S,K) -> Delta
        SID, // (S,σ) -> Delta
        STD, // (S,T) -> Delta
        KTD, // (K,T) -> Delta
        ITD, // (σ,T) -> Delta

        SKG, // (S,K) -> Gamma
        SIG, // (S,σ) -> Gamma
        STG, // (S,T) -> Gamma

        SKV, // (S,K) -> Vega
        SIV, // (S,σ) -> Vega
        STV, // (S,T) -> Vega
        KTV, // (K,T) -> Vega
        ITV, // (σ,T) -> Vega

        STH, // (S,T) -> Theta
        SIH, // (S,σ) -> Theta

        STO, // (S,T) -> Rho
        KTO, // (K,T) -> Rho

        STM, // (S,T) -> Implied Volatility
    };

    enum class InputType {
        NONE, // Do not show
        SINGLE, // Show constant value
        RANGE // Show range between two values
    };

    struct SurfaceConfig {
        char xVal;
        char yVal;
        char zVal;

        QString xLabel;
        QString yLabel;
        QString zLabel;

        InputType input_S;
        InputType input_K;
        InputType input_r;
        InputType input_q;
        InputType input_sigma;
        InputType input_T;

        std::function<double(double S, double K, double r, double q, double sigma, double T)> computeZ;
    };

    static std::unordered_map<SurfaceMode, SurfaceConfig> surfaceMap;
};

#endif // SURFACE_H
