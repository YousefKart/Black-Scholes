#ifndef RANGESLIDER_H
#define RANGESLIDER_H

#include <QSlider>

class RangeSlider : public QSlider
{
    Q_OBJECT

public:
    explicit RangeSlider(Qt::Orientation orientation, QWidget* parent = nullptr);

    int lowerValue() const { return m_lower; }
    int upperValue() const { return m_upper; }

    void setLowerValue(int value);
    void setUpperValue(int value);
    void setRange(int min, int max);

signals:
    void lowerValueChanged(int);
    void upperValueChanged(int);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_lower;
    int m_upper;
    enum Handle { NoHandle, LowerHandle, UpperHandle };
    Handle m_activeHandle = NoHandle;

    int pixelPosToRangeValue(int pos) const;
};

#endif
