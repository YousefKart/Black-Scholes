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

    // Plot
    QCustomPlot* plot() const { return m_plot; }
    QCPColorMap* colorMap() const { return m_colorMap; }
    QCPColorScale* colorScale() const { return m_colorScale; }

    // Left-Hand Menu
    QPushButton* toggle_CP() const { return m_toggle_CP; }
    QButtonGroup* buttonGroup() const { return m_buttonGroup; }

    // User-Input Variables
    QDoubleSpinBox* spin_S() const { return m_spin_S; }
    QDoubleSpinBox* spin_K() const { return m_spin_K; }
    QDoubleSpinBox* spin_r() const { return m_spin_r; }
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
