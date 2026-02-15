#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "qcustomplot.h"

class Component : public QWidget
{
    Q_OBJECT

public:
    explicit Component(QWidget* parent = nullptr);

    static constexpr int SLIDER_RESOLUTION = 1000;
    static constexpr double minValue_S = 0.01;
    static constexpr double maxValue_S = 1000000.0;
    static constexpr double minValue_K = 0.01;
    static constexpr double maxValue_K = 1000000.0;
    static constexpr double minValue_r = 0.0;
    static constexpr double maxValue_r = 100.0;
    static constexpr double minValue_q = 0.0; // Not defined yet
    static constexpr double maxValue_q = 0.0; // Not defined yet
    static constexpr double minValue_sigma = 0.0001;
    static constexpr double maxValue_sigma = 5.0;
    static constexpr double minValue_T = 0.0001;
    static constexpr double maxValue_T = 10000.0;

    // Convert from spin to slider or vice versa
    static int spinToSliderLinear(double stepVal, double min, double max, int steps);
    static int spinToSliderLog(double stepVal, double min, double max, int steps);
    static double sliderToSpinLinear(int sliderVal, double min, double max, int steps);
    static double sliderToSpinLog(int sliderVal, double min, double max, int steps);

    // Plot
    QCustomPlot* plot() const { return m_plot; }
    QCPColorMap* colorMap() const { return m_colorMap; }
    QCPColorScale* colorScale() const { return m_colorScale; }

    // Left-Hand Menu
    QPushButton* toggle_CP() const { return m_toggle_CP; }
    QButtonGroup* buttonGroup() const { return m_buttonGroup; }

    // User-Input Variables
    QSlider* slider_S() const { return m_slider_S; }
    QDoubleSpinBox* spin_S() const { return m_spin_S; }
    QSlider* slider_K() const { return m_slider_K; }
    QDoubleSpinBox* spin_K() const { return m_spin_K; }
    QSlider* slider_r() const { return m_slider_r; }
    QDoubleSpinBox* spin_r() const { return m_spin_r; }
    QSlider* slider_sigma() const { return m_slider_sigma; }
    QDoubleSpinBox* spin_sigma() const { return m_spin_sigma; }
    QDoubleSpinBox* spinmin_T() const { return m_spinmin_T; }
    QDoubleSpinBox* spinmax_T() const { return m_spinmax_T; }


private:
    void setupUI();
    void setupPlot();

    // Left-Hand Menu
    QLabel* m_menuTitle;
    QPushButton* m_button_STP;
    QPushButton* m_button_SSP;
    QButtonGroup* m_buttonGroup;

    // Plot
    QCustomPlot* m_plot;
    QCPColorMap* m_colorMap;
    QCPColorScale* m_colorScale;

    // Price Display Switch (Calls/Puts)
    QPushButton* m_toggle_CP;

    // S Components
    QHBoxLayout* m_layout_S;
    QSlider* m_slider_S;
    QDoubleSpinBox* m_spin_S; // Stock Price

    // K Components
    QHBoxLayout* m_layout_K;
    QSlider* m_slider_K;
    QDoubleSpinBox* m_spin_K; // Strike Price

    // r Components
    QHBoxLayout* m_layout_r;
    QSlider* m_slider_r;
    QDoubleSpinBox* m_spin_r; // Risk-Free Rate (Annual)

    // σ Components
    QHBoxLayout* m_layout_sigma;
    QSlider* m_slider_sigma;
    QDoubleSpinBox* m_spin_sigma; // Volatility (Annual)

    // T Components
    QHBoxLayout* m_layout_T;  // Horizontal Layout
    QDoubleSpinBox* m_spinmin_T; // min_inum Time to Expiry (Days)
    QDoubleSpinBox* m_spinmax_T; // max_imum Time to Expiry (Days)

    // Main Layout
    QVBoxLayout* m_leftLayout;
    QVBoxLayout* m_rightLayout;
    QHBoxLayout* m_mainLayout;
};

#endif // COMPONENT_H
