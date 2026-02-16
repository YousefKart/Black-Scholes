#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "qcustomplot.h"
#include "rangeslider.h"
#include "surface.h"

class Component : public QWidget
{
    Q_OBJECT

public:
    explicit Component(QWidget* parent = nullptr);

    static constexpr int SLIDER_RESOLUTION = 1000;
    static constexpr double minLimit_S = 0.01;
    static constexpr double maxLimit_S = 1000000.0;
    static constexpr double minLimit_K = 0.01;
    static constexpr double maxLimit_K = 1000000.0;
    static constexpr double minLimit_r = 0.0;
    static constexpr double maxLimit_r = 100.0;
    static constexpr double minLimit_q = 0.0;
    static constexpr double maxLimit_q = 100.0;
    static constexpr double minLimit_sigma = 0.0001;
    static constexpr double maxLimit_sigma = 5.0;
    static constexpr double minLimit_T = 0.0001;
    static constexpr double maxLimit_T = 10000.0;

    // Convert from spin to slider or vice versa
    static int spinToSliderLinear(double stepVal, double min, double max, int steps);
    static int spinToSliderLog(double stepVal, double min, double max, int steps);
    static double sliderToSpinLinear(int sliderVal, double min, double max, int steps);
    static double sliderToSpinLog(int sliderVal, double min, double max, int steps);

    // Set UI visibility based on surface config
    void setConfig(Surface::SurfaceConfig config);

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
    RangeSlider* rangeSlider_S() const { return m_rangeSlider_S; }
    QDoubleSpinBox* spinMin_S() const { return m_spinMin_S; }
    QDoubleSpinBox* spinMax_S() const { return m_spinMax_S; }

    QSlider* slider_K() const { return m_slider_K; }
    QDoubleSpinBox* spin_K() const { return m_spin_K; }
    QSlider* rangeSlider_K() const { return m_rangeSlider_K; }
    QDoubleSpinBox* spinMin_K() const { return m_spinMin_K; }
    QDoubleSpinBox* spinMax_K() const { return m_spinMax_K; }

    QSlider* slider_r() const { return m_slider_r; }
    QDoubleSpinBox* spin_r() const { return m_spin_r; }
    QSlider* rangeSlider_r() const { return m_rangeSlider_r; }
    QDoubleSpinBox* spinMin_r() const { return m_spinMin_r; }
    QDoubleSpinBox* spinMax_r() const { return m_spinMax_r; }

    QSlider* slider_q() const { return m_slider_q; }
    QDoubleSpinBox* spin_q() const { return m_spin_q; }
    QSlider* rangeSlider_q() const { return m_rangeSlider_q; }
    QDoubleSpinBox* spinMin_q() const { return m_spinMin_q; }
    QDoubleSpinBox* spinMax_q() const { return m_spinMax_q; }

    QSlider* slider_sigma() const { return m_slider_sigma; }
    QDoubleSpinBox* spin_sigma() const { return m_spin_sigma; }
    QSlider* rangeSlider_sigma() const { return m_rangeSlider_sigma; }
    QDoubleSpinBox* spinMin_sigma() const { return m_spinMin_sigma; }
    QDoubleSpinBox* spinMax_sigma() const { return m_spinMax_sigma; }

    QSlider* slider_T() const { return m_slider_T; }
    QDoubleSpinBox* spin_T() const { return m_spin_T; }
    QSlider* rangeSlider_T() const { return m_rangeSlider_T; }
    QDoubleSpinBox* spinMin_T() const { return m_spinMin_T; }
    QDoubleSpinBox* spinMax_T() const { return m_spinMax_T; }


private:
    void setupUI();
    void setupPlot();

    Surface::SurfaceConfig config;

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

    // S Components (Stock Price)
    QHBoxLayout* m_layout_S;
    QSlider* m_slider_S;
    QDoubleSpinBox* m_spin_S;
    RangeSlider* m_rangeSlider_S;
    QDoubleSpinBox* m_spinMin_S;
    QDoubleSpinBox* m_spinMax_S;

    // K Components (Strike Price)
    QHBoxLayout* m_layout_K;
    QSlider* m_slider_K;
    QDoubleSpinBox* m_spin_K;
    QSlider* m_rangeSlider_K; // TODO: Implement with a real range class
    QDoubleSpinBox* m_spinMin_K;
    QDoubleSpinBox* m_spinMax_K;

    // r Components (Risk-Free Rate (Annual))
    QHBoxLayout* m_layout_r;
    QSlider* m_slider_r;
    QDoubleSpinBox* m_spin_r;
    QSlider* m_rangeSlider_r; // TODO: Implement with a real range class
    QDoubleSpinBox* m_spinMin_r;
    QDoubleSpinBox* m_spinMax_r;

    // q Components (Dividen Yield (Annual))
    QHBoxLayout* m_layout_q;
    QSlider* m_slider_q;
    QDoubleSpinBox* m_spin_q;
    QSlider* m_rangeSlider_q; // TODO: Implement with a real range class
    QDoubleSpinBox* m_spinMin_q;
    QDoubleSpinBox* m_spinMax_q;

    // σ Components (Volatility (Annual))
    QHBoxLayout* m_layout_sigma;
    QSlider* m_slider_sigma;
    QDoubleSpinBox* m_spin_sigma;
    QSlider* m_rangeSlider_sigma; // TODO: Implement with a real range class
    QDoubleSpinBox* m_spinMin_sigma;
    QDoubleSpinBox* m_spinMax_sigma;

    // T Components
    QHBoxLayout* m_layout_T;
    QSlider* m_slider_T;
    QDoubleSpinBox* m_spin_T;
    QSlider* m_rangeSlider_T; // TODO: Implement with a real range class
    QDoubleSpinBox* m_spinMin_T;
    QDoubleSpinBox* m_spinMax_T;

    // Main Layout
    QVBoxLayout* m_leftLayout;
    QVBoxLayout* m_rightLayout;
    QHBoxLayout* m_mainLayout;
};

#endif // COMPONENT_H
