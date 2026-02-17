#include "surface.h"
#include "functions.h"

Surface::Surface() {}

std::unordered_map<Surface::SurfaceMode, Surface::SurfaceConfig> Surface::surfaceMap = {
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

            [] (double S, double K, double r, double q, double sigma, double T) {
                double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                double d2 = Functions::computeD2(sigma, T, d1);
                return Functions::computeCall(S, K, r, q, T, d1, d2);
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

            [] (double S, double K, double r, double q, double sigma, double T) {
                double d1 = Functions::computeD1(S, K, r, q, sigma, T);
                double d2 = Functions::computeD2(sigma, T, d1);
                return Functions::computeCall(S, K, r, q, T, d1, d2);
            }
        }
    }
};
