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
    m_roll = euler.x();
    update();
}
void ArtificialHorizon::paintEvent(QPaintEvent* event) {

    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int32_t w = width();
    int32_t h = height();
    int32_t centerX = w / 2;
    int32_t centerY = h / 2;
    int32_t radius = std::min(w, h) / 2 - 75;

    painter.fillRect(rect(), Color::SkyBlue); // Sky blue background
    int32_t ground_y =
        centerY + static_cast<int32_t>(m_pitch * radius / (M_PI / 2));
    QColor ColorEarthOrange(181, 96, 55);
    painter.fillRect(0, ground_y, w, h - ground_y,
                     Color::EarthOrange); // Ground
    painter.fillRect(0, ground_y - 2, w, 4, Qt::white);

    // center dot and wings and circle
    painter.setPen(QPen(Color::HorizonGray, 2));
    // painter.drawEllipse(centerX - radius, centerY - radius, radius * 2,
    //                     radius * 2);
    const QColor& plane_color = Color::HorizonGray;
    painter.fillRect(centerX - 20, centerY - 1, 10, 2, plane_color);
    painter.fillRect(centerX + 10, centerY - 1, 10, 2, plane_color);
    painter.fillRect(centerX - 3, centerY - 3, 6, 6, plane_color);
    // pitch ladder

    painter.save();
    painter.setPen(QPen(QColor(0,0,0), 1));
    painter.setFont(QFont("Arial", 8, QFont::Bold));
    constexpr int32_t line_length = 20;
    for (int32_t i = -360; i <= 360; i += 10) {
        if (i == 0)
            continue;
        int32_t y = centerY + static_cast<int32_t>(i * radius / 90.0) + static_cast<int32_t>(m_pitch * radius / (M_PI / 2));
        if (y > centerY - radius && y < centerY + radius){
            painter.drawLine(centerX - line_length, y, centerX + line_length, y);
            if (i % 30 == 0) {
                painter.drawText(centerX + line_length + 5, y + 4,
                                 QString::number(-i));
            }

        }
    }
    painter.restore();
}
