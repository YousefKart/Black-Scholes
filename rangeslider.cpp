#include "RangeSlider.h"
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QMouseEvent>

RangeSlider::RangeSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    m_lower = minimum();
    m_upper = maximum();
}

void RangeSlider::setLowerValue(int value)
{
    value = qBound(minimum(), value, m_upper);
    if (value != m_lower)
    {
        m_lower = value;
        emit lowerValueChanged(m_lower);
        update();
    }
}

void RangeSlider::setUpperValue(int value)
{
    value = qBound(m_lower, value, maximum());
    if (value != m_upper)
    {
        m_upper = value;
        emit upperValueChanged(m_upper);
        update();
    }
}

void RangeSlider::paintEvent(QPaintEvent*)
{
    QStylePainter painter(this);
    QStyleOptionSlider option;
    initStyleOption(&option);

    option.subControls = QStyle::SC_SliderGroove;
    option.state &= ~QStyle::State_Sunken;
    option.subControls = QStyle::SC_SliderGroove;
    painter.drawComplexControl(QStyle::CC_Slider, option);

    option.sliderPosition = m_lower;
    QRect lowerRect = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, this);

    option.sliderPosition = m_upper;
    QRect upperRect = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, this);

    QRect groove = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderGroove, this);

    QRect highlight;
    int thickness = 3;

    if (orientation() == Qt::Horizontal)
    {
        int y = groove.center().y() - thickness / 2;

        highlight = QRect(
            lowerRect.center().x(),
            y,
            upperRect.center().x() - lowerRect.center().x(),
            thickness
            );
    }
    else
    {
        int x = groove.center().x() - thickness / 2;

        highlight = QRect(
            x,
            upperRect.center().y(),
            thickness,
            lowerRect.center().y() - upperRect.center().y()
            );
    }

    highlight = highlight.normalized();

    QColor accent = option.palette.color(QPalette::Highlight);
    painter.setBrush(accent);
    painter.setPen(Qt::NoPen);
    painter.drawRect(highlight);

    option.sliderPosition = m_lower;
    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);

    option.sliderPosition = m_upper;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}


void RangeSlider::mousePressEvent(QMouseEvent* event)
{
    QStyleOptionSlider option;
    initStyleOption(&option);

    option.sliderPosition = m_lower;
    QRect lowerRect = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, this);

    option.sliderPosition = m_upper;
    QRect upperRect = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, this);

    if (lowerRect.contains(event->pos()))
        m_activeHandle = LowerHandle;
    else if (upperRect.contains(event->pos()))
        m_activeHandle = UpperHandle;
    else
    {
        int clickValue = pixelPosToRangeValue(
            orientation() == Qt::Horizontal
                ? event->pos().x()
                : event->pos().y());

        if (std::abs(clickValue - m_lower) <
            std::abs(clickValue - m_upper))
            m_activeHandle = LowerHandle;
        else
            m_activeHandle = UpperHandle;
    }

}

void RangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (m_activeHandle == NoHandle)
        return;

    int value = pixelPosToRangeValue(
        orientation() == Qt::Horizontal ? event->pos().x()
                                        : event->pos().y());

    if (m_activeHandle == LowerHandle)
        setLowerValue(value);
    else if (m_activeHandle == UpperHandle)
        setUpperValue(value);
}

int RangeSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider option;
    const_cast<RangeSlider*>(this)->initStyleOption(&option);

    QRect groove = style()->subControlRect(
        QStyle::CC_Slider, &option,
        QStyle::SC_SliderGroove, this);

    int sliderMin = orientation() == Qt::Horizontal
                        ? groove.left()
                        : groove.top();
    int sliderMax = orientation() == Qt::Horizontal
                        ? groove.right()
                        : groove.bottom();

    return QStyle::sliderValueFromPosition(
        minimum(), maximum(),
        pos - sliderMin,
        sliderMax - sliderMin);
}
