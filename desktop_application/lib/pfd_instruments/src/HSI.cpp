#include "HSI.hpp"
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <cmath>
namespace Colors {
inline const QColor Magenta("#ff00ff");
inline const QColor MatteDarkGray("#151515");
}; // namespace Colors

HSI::HSI(QWidget* parent)
    : QFrame(parent), m_current_heading(0.0), m_current_crs(0.0) {
    setStyleSheet("background-color: #1a1a1a;");
}
void HSI::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int32_t center_x = width() / 2;
    const int32_t center_y = height() / 2;
    const int32_t radius = std::min(center_x, center_y) - 35;

    painter.save();
    painter.translate(center_x, center_y);
    painter.rotate(-m_current_heading);

    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPointF(0, 0), radius, radius);

    // compass rose
    for (int32_t angle = 0; angle < 360; angle += 10) {
        painter.save();
        painter.rotate(angle);

        if (angle % 30 == 0) {
            painter.setPen(QPen(Qt::white, 2));
            painter.drawLine(0, radius, 0, radius - 12);

            QString label;
            if (angle == 0)
                label = "N";
            else if (angle == 90)
                label = "E";
            else if (angle == 180)
                label = "S";
            else if (angle == 270)
                label = "W";
            else
                label = QString::number(angle);

            painter.setFont(QFont("Arial", 9, QFont::Bold));
            const QRect textRect(-15, -(radius - 28), 30, 15);

            painter.drawText(textRect, Qt::AlignCenter, label);
        } else {
            painter.setPen(QPen(Qt::gray, 1));
            const QLine tick_line(0, radius, 0, radius - 7);
            painter.drawLine(tick_line);
        }
        painter.restore();
    }

    painter.save();
    painter.rotate(m_current_crs);
    painter.setPen(QPen(Colors::Magenta, 2));

    const QLine crs_line(0, -radius, 0, radius);
    painter.drawLine(crs_line);

    QPolygon arrowhead;
    arrowhead << QPoint(0, -radius) << QPoint(-6, -(radius - 12))
              << QPoint(6, -(radius - 12));
    painter.setBrush(Colors::Magenta);
    painter.drawPolygon(arrowhead);

    painter.restore();
    painter.restore();

    painter.save();
    painter.translate(center_x, center_y);
    painter.setPen(
        QPen(Qt::yellow, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);

    QPainterPath airplane;
    airplane.moveTo(0, -12);
    airplane.lineTo(0, 12);
    airplane.moveTo(-18, -2);
    airplane.lineTo(18, -2);
    airplane.moveTo(-6, 8);
    airplane.lineTo(6, 8);
    painter.drawPath(airplane);

    painter.restore();

    const QRect hdgBox(center_x - 25, 2, 50, 20);
    painter.setPen(QPen(Qt::darkGray, 1));
    painter.setBrush(Colors::MatteDarkGray);
    painter.drawRect(hdgBox);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    const QString hdgText =
        QString("%1").arg(static_cast<int16_t>(std::round(m_current_heading)),
                          3, 10, QChar('0'));
    painter.drawText(hdgBox, Qt::AlignCenter, hdgText);
}

void HSI::setCurrentHeading(double heading) {
    m_current_heading = heading;
    update();
}

void HSI::setCurrentCRS(int16_t crs) {
    m_current_crs = crs;
    update();
}
