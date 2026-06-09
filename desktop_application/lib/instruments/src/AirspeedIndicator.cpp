#include "AirspeedIndicator.hpp"
#include <QPaintEvent>
#include <QPainter>

AirspeedIndicator::AirspeedIndicator(QWidget* parent)
    : QFrame(parent), m_current_airspeed(0.0) {
    setMinimumWidth(60);
    setStyleSheet("background-color: #1a1a1a;");
}

void AirspeedIndicator::updateAirspeed(double airspeed) {
    m_current_airspeed = airspeed;
    this->update();
}

void AirspeedIndicator::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const int32_t w = width();
    const int32_t h = height();
    const int32_t center_y = h / 2;

    constexpr double pixels_per_knot = 2.0;
    constexpr int8_t tick_interval = 20;

    const int32_t range = (h / 2.0) / pixels_per_knot;

    const std::div_t div_res = std::div(
        static_cast<int32_t>(m_current_airspeed - range), tick_interval);
    const int32_t min_val = div_res.quot * tick_interval;
    const int32_t max_val = static_cast<int32_t>(m_current_airspeed + range);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);

    for (int32_t val = min_val; val <= max_val; val += tick_interval) {
        if (val < 0)
            continue; // ograniczenia holonomiczne robią brrr

        const double y_offset = (val - m_current_airspeed) * pixels_per_knot;
        const int32_t y = static_cast<int32_t>(center_y - y_offset);

        painter.drawLine(w - 15, y, w, y);
        if (val % 20 == 0) {
            const QRect text_rect(0, y - 10, w - 20, 20);
            painter.drawText(text_rect, Qt::AlignRight | Qt::AlignVCenter,
                             QString::number(val));
        }
    }

    painter.setBrush(Qt::black);
    painter.setPen(QPen(Qt::white, 2));

    QPolygon pointer;
    pointer << QPoint(w - 15, center_y) << QPoint(w - 25, center_y - 15)
            << QPoint(5, center_y - 15) << QPoint(5, center_y + 15)
            << QPoint(w - 25, center_y + 15);
    painter.drawPolygon(pointer);
    painter.drawLine(w - 15, center_y, w - 5, center_y);

    font.setBold(true);
    font.setPixelSize(18);
    painter.setFont(font);
    const QRect pointer_text_rect(10, center_y - 15, w - 35, 30);
    painter.drawText(pointer_text_rect, Qt::AlignCenter,
                     QString::number(std::round(m_current_airspeed)));
}
