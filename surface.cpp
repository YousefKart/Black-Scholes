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
                double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                double d2 = Functions::computeD2(sigma, T, d1);
                return mode == OptionMode::PUT
                           ? Functions::computePut(S, K, r, q, T, d1, d2)
                           : Functions::computeCall(S, K, r, q, T, d1, d2);
            }
        }
    },

    {
        Surface::SurfaceMode::SRP, // (S,r) -> Price
        {
            'R', 'S', 'P',
            "Annual Risk-Free Rate (r)", "Stock Price (S)", "Option Price",
            Surface::InputType::RANGE, // Stock Price
            Surface::InputType::SINGLE,
            Surface::InputType::RANGE, // Risk-Free Rate
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,
            Surface::InputType::SINGLE,

            [] (OptionMode mode, double S, double K, double r, double q, double sigma, double T) {
                double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                double d2 = Functions::computeD2(sigma, T, d1);
                return mode == OptionMode::PUT
                           ? Functions::computePut(S, K, r, q, T, d1, d2)
                           : Functions::computeCall(S, K, r, q, T, d1, d2);
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
                    double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                    double d2 = Functions::computeD2(sigma, T, d1);
                    return mode == OptionMode::PUT
                               ? Functions::computePut(S, K, r, q, T, d1, d2)
                               : Functions::computeCall(S, K, r, q, T, d1, d2);
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
                double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                double d2 = Functions::computeD2(sigma, T, d1);
                return mode == OptionMode::PUT
                           ? Functions::computePut(S, K, r, q, T, d1, d2)
                           : Functions::computeCall(S, K, r, q, T, d1, d2);
            }
        }
    },
};
