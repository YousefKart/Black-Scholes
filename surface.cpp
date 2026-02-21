#include "surface.h"
#include "functions.h"

Surface::Surface() {}

std::unordered_map<Surface::SurfaceMode, Surface::SurfaceConfig> Surface::surfaceMap = {

    {
        Surface::SurfaceMode::SKP, // (S,K) -> Price
        {
            'K', 'S', 'P',
            "Strike Price (K)", "Stock Price (S)", "Option Price",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::RANGE, // Strike Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutPrice(S, K, r, q, sigma, T)
                           : Functions::computeCallPrice(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::SIP, // (S,σ) -> Price
        {
            'I', 'S', 'P',
                "Volatility (\u03C3)", "Stock Price (S)", "Option Price",
                Surface::InputType::RANGE, // Stock Price
                Surface::InputType::SINGLE,
                Surface::InputType::SINGLE,
                Surface::InputType::SINGLE,
                Surface::InputType::RANGE, // Volatility
                Surface::InputType::SINGLE,

                [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                    return mode == OptionMode::PUT
                               ? Functions::computePutPrice(S, K, r, q, sigma, T)
                               : Functions::computeCallPrice(S, K, r, q, sigma, T);
                }
        }
    },

    {
        Surface::SurfaceMode::STP, // (S,T) -> Price
        {
            'T', 'S', 'P',
            "Years (T)", "Stock Price (S)", "Option Price",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutPrice(S, K, r, q, sigma, T)
                           : Functions::computeCallPrice(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::SID, // (S,σ) -> Delta
        {
            'I', 'S', 'D',
            "Volatility (\u03C3)", "Stock Price (S)", "Delta",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Volatility
            Surface::InputType::SINGLE,

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutDelta(S, K, r, q, sigma, T)
                           : Functions::computeCallDelta(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STD, // (S,T) -> Delta
        {
            'T', 'S', 'D',
            "Time (Years)", "Stock Price (S)", "Delta",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutDelta(S, K, r, q, sigma, T)
                           : Functions::computeCallDelta(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STG, // (S,T) -> Gamma
        {
            'T', 'S', 'G',
            "Time (Years)", "Stock Price (S)", "Gamma",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return Functions::computeGamma(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STV, // (S,T) -> Vega
        {
            'T', 'S', 'V',
            "Time (Years)", "Stock Price (S)", "Vega",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return Functions::computeVega(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STH, // (S,T) -> Theta
        {
            'T', 'S', 'H',
            "Time (Years)", "Stock Price (S)", "Theta",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutTheta(S, K, r, q, sigma, T)
                           : Functions::computeCallTheta(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STO, // (S,T) -> Rho
        {
            'T', 'S', 'O',
            "Time (Years)", "Stock Price (S)", "Rho",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                           ? Functions::computePutRho(S, K, r, q, sigma, T)
                           : Functions::computeCallRho(S, K, r, q, sigma, T);
            }
        }
    },

    {
        Surface::SurfaceMode::STM, // (S,T) -> Implied Volatility
        {
            'T', 'S', 'M',
            "Time (Years)", "Stock Price (S)", "Implied Volatility",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Time

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                return mode == OptionMode::PUT
                        ? Functions::computePutIV(S, K, r, q, Functions::computePutPrice(S, K, r, q, sigma, T), T)
                           : Functions::computeCallIV(S, K, r, q, Functions::computeCallPrice(S, K, r, q, sigma, T), T);
            }
        }
    },
};
