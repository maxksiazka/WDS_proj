#include "ArtificialHorizon.hpp"
namespace Color {
inline const QColor SkyBlue(135, 206, 235);
inline const QColor EarthOrange(181, 96, 55);
inline const QColor HorizonGray(50, 50, 50);
} // namespace Color

ArtificialHorizon::ArtificialHorizon(QWidget* parent) : QFrame(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet(
        "QFrame { background-color: #0a0a0a; border-radius: 2px solid #333; }");
    setMinimumSize(300, 300);
}

void ArtificialHorizon::updateOrientation(
    const Eigen::Quaterniond& orientation) {
    qDebug() << "Orientation updated: " << orientation.w() << orientation.x()
             << orientation.y() << orientation.z();
    Eigen::Vector3d euler =
        orientation.toRotationMatrix().canonicalEulerAngles(2, 1, 0);
    m_pitch = euler.y();
    m_roll = euler.z();
    qDebug() << "Pitch: " << m_pitch << " Roll: " << m_roll;
    update();
}
void ArtificialHorizon::paintEvent(QPaintEvent* event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int32_t w = width();
    const int32_t h = height();
    const int32_t centerX = w / 2;
    const int32_t centerY = h / 2;
    const int32_t radius = std::min(w, h) / 2 - 75;

    painter.save();
    painter.translate(centerX, centerY);
    double roll_degrees = -m_roll * (180.0 / M_PI);
    painter.rotate(roll_degrees);
    const int32_t pitch_offset =
        static_cast<int32_t>(m_pitch * radius / (M_PI / 2));
    const int32_t side = static_cast<int32_t>(std::sqrt(w * w + h * h)) * 2;

    painter.fillRect(-side / 2, -side / 2, side, side, Color::SkyBlue);
    painter.fillRect(-side / 2, pitch_offset, side, side / 2,
                     Color::EarthOrange);
    painter.fillRect(-side / 2, pitch_offset - 2, side, 4, Qt::white);

    painter.setPen(QPen(Qt::black, 1));
    painter.setFont(QFont("Arial", 8, QFont::Bold));
    constexpr int32_t line_length = 20;

    for (int32_t i = -360; i <= 360; i += 10) {
        if (i == 0)
            continue;

        int32_t y = pitch_offset + static_cast<int32_t>(i * radius / 90.0);

        if (y > -radius && y < radius) {
            painter.drawLine(-line_length, y, line_length, y);
            if (i % 30 == 0) {
                painter.drawText(line_length + 5, y + 4, QString::number(-i));
            }
        }
    }
    painter.setPen(QPen(Color::HorizonGray, 2));

    QRect arc_rect(-radius, -radius, radius * 2, radius * 2);
    static constexpr int16_t MIN_ARC = 30;
    static constexpr int16_t ARC_SPAN = 120;
    painter.drawArc(arc_rect, MIN_ARC * 16, ARC_SPAN * 16);

    static constexpr int32_t ROLL_TICKS[] = {-60, -45, -30, -20, -10, 0,
                                      10,  20,  30,  45,  60};
    for (const int32_t& angle : ROLL_TICKS) {
        painter.save();
        painter.rotate(angle);
        int32_t tickLength;
        if (angle == 0 || std::abs(angle) == 30 || std::abs(angle) == 60)
            tickLength = 12;
        else
            tickLength = 6;
        painter.drawLine(0, -radius, 0, -radius - tickLength);

        if (std::abs(angle) == 30 || std::abs(angle) == 60) {
            painter.setFont(QFont("Arial", 7, QFont::Normal));
            QString label = QString::number(std::abs(angle));
            painter.drawText(QRect(-15, -radius - tickLength - 14, 30, 12),
                             Qt::AlignCenter, label);
        }
        painter.restore();
    }
    painter.restore();
    // plane
    QRect left_wing(centerX - 20, centerY - 1, 10, 2);
    QRect right_wing(centerX + 10, centerY - 1, 10, 2);
    QRect center_rect(centerX - 3, centerY - 3, 6, 6);
    painter.fillRect(left_wing, Color::HorizonGray);
    painter.fillRect(right_wing, Color::HorizonGray);
    painter.fillRect(center_rect, Color::HorizonGray);
    // roll indicator
    QPolygon roll_indicator;
    roll_indicator << QPoint(0, -radius - 3) << QPoint(-9, -radius - 15)
                   << QPoint(9, -radius - 15);
    painter.translate(centerX, centerY);
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Color::HorizonGray);
    painter.drawPolygon(roll_indicator);
}
