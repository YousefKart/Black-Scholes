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
    QPushButton* toggleCP() const { return m_toggleCP; }
    QButtonGroup* buttonGroup() const { return m_buttonGroup; }

    // User-Input Variables
    QDoubleSpinBox* spinS() const { return m_spinS; }
    QDoubleSpinBox* spinK() const { return m_spinK; }
    QDoubleSpinBox* spinR() const { return m_spinR; }
    QDoubleSpinBox* spinSigma() const { return m_spinSigma; }
    QDoubleSpinBox* spinMinT() const { return m_spinMinT; }
    QDoubleSpinBox* spinMaxT() const { return m_spinMaxT; }


private:
    void setupUI();
    void setupPlot();

    // Left-Hand Menu
    QLabel* m_menuTitle;
    QPushButton* m_buttonSTP;
    QPushButton* m_buttonSSP;
    QButtonGroup* m_buttonGroup;

    // Plot
    QCustomPlot* m_plot;
    QCPColorMap* m_colorMap;
    QCPColorScale* m_colorScale;

    // Price Display Switch (Calls/Puts)
    QPushButton* m_toggleCP;

    // S Components
    QDoubleSpinBox* m_spinS; // Stock Price

    // K Components
    QDoubleSpinBox* m_spinK; // Strike Price

    // r Components
    QDoubleSpinBox* m_spinR; // Risk-Free Rate (Annual)

    // σ Components
    QDoubleSpinBox* m_spinSigma; // Volatility (Annual)

    // T Components
    QDoubleSpinBox* m_spinMinT; // Mininum Time to Expiry (Days)
    QDoubleSpinBox* m_spinMaxT; // Maximum Time to Expiry (Days)
    QHBoxLayout* m_rangeLayoutT;  // Horizontal Layout

    // Main Layout
    QVBoxLayout* m_leftLayout;
    QVBoxLayout* m_rightLayout;
    QHBoxLayout* m_mainLayout;
};

#endif // COMPONENT_H
