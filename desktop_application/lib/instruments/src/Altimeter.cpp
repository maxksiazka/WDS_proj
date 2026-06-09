#include "Altimeter.hpp"
#include <QPaintEvent>
#include <QPainter>

Altimeter::Altimeter(QWidget* parent)
    : QFrame(parent), m_current_altitude(0.0) {
    setMinimumWidth(60);
    setStyleSheet("background-color: #1a1a1a;");
}

void Altimeter::updateAltitude(double altitude) {
    m_current_altitude = altitude;
    this->update();
}

void Altimeter::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const int32_t w = width();
    const int32_t h = height();
    const int32_t center_y = h / 2;

    constexpr double pixels_per_knot = 2.0;
    constexpr int8_t tick_interval = 20;

    const int32_t range = (h / 2.0) / pixels_per_knot;

    const std::div_t div_res = std::div(
        static_cast<int32_t>(m_current_altitude - range), tick_interval);
    const int32_t min_val = div_res.quot * tick_interval;
    const int32_t max_val = static_cast<int32_t>(m_current_altitude + range);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);

    for (int32_t val = min_val; val <= max_val; val += tick_interval) {
        if (val < 0)
            continue; // ograniczenia holonomiczne robią brrr

        const double y_offset = (val - m_current_altitude) * pixels_per_knot;
        const int32_t y = static_cast<int32_t>(center_y - y_offset);

        const QLine tick_line(0, y, 15, y);
        painter.drawLine(tick_line);
        if (val % 20 == 0) {
            const QRect tick_text_rect(20, y - 10, w, 20);
            painter.drawText(tick_text_rect, Qt::AlignLeft | Qt::AlignVCenter,
                             QString::number(val));
        }
    }

    painter.setBrush(Qt::black);
    painter.setPen(QPen(Qt::white, 2));

    QPolygon pointer;
    pointer << QPoint(15, center_y) << QPoint(25, center_y - 15)
            << QPoint(w - 5, center_y - 15) << QPoint(w - 5, center_y + 15)
            << QPoint(25, center_y + 15);
    painter.drawPolygon(pointer);

    const QLine pointer_line(15, center_y, 5, center_y);
    painter.drawLine(pointer_line);

    font.setBold(true);
    font.setPixelSize(18);
    painter.setFont(font);

    const QRect pointer_text_rect(25, center_y - 15, w - 35, 30);
    painter.drawText(pointer_text_rect, Qt::AlignLeft | Qt::AlignVCenter,
                     QString::number(std::round(m_current_altitude)));
}
