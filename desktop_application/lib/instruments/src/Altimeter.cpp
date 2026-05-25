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
    int32_t w = width();
    int32_t h = height();
    int32_t center_y = h / 2;

    constexpr double pixels_per_knot = 2.0;
    constexpr int8_t tick_interval = 20;

    int32_t range = (h / 2.0) / pixels_per_knot;
    int32_t min_val = static_cast<int32_t>(m_current_altitude - range);
    int32_t max_val = static_cast<int32_t>(m_current_altitude + range);

    min_val = (min_val / tick_interval) * tick_interval;

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);

    for (int32_t val = min_val; val <= max_val; val += tick_interval) {
        if (val < 0)
            continue; // ograniczenia holonomiczne robią brrr

        double y_offset = (val - m_current_altitude) * pixels_per_knot;
        int32_t y = static_cast<int32_t>(center_y - y_offset);

        painter.drawLine(0, y, 15, y);
        if (val % 20 == 0) {
            painter.drawText(20, y - 10, w, 20,
                             Qt::AlignLeft | Qt::AlignVCenter,
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
    painter.drawLine(15, center_y, 5, center_y);

    font.setBold(true);
    font.setPixelSize(18);
    painter.setFont(font);
    painter.drawText(25, center_y - 15, w - 35, 30,Qt::AlignLeft | Qt::AlignVCenter,
                     QString::number(std::round(m_current_altitude)));
}
