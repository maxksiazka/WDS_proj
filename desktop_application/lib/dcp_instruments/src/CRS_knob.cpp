#include "CRS_knob.hpp"
namespace Colors {
static const QColor cyan("#00a8e8");
static const QColor dark_gray("#1a1a1a");
} // namespace Colors

CRS_knob::CRS_knob(QWidget* parent) : QDial(parent) {
    setMinimumSize(40, 40);
    setRange(0, 360);
    setValue(0);
    setNotchesVisible(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void CRS_knob::paintEvent(QPaintEvent* event) {

    QPainter painter(this);
    // painter.setPen(Qt::blue);
    //     painter.drawRect(rect().adjusted(0,0,-1,-1));
    painter.setRenderHint(QPainter::Antialiasing);

    int32_t size = width();
    QRect rect(0, 0, size, size);
    QPoint center = rect.center();
    int32_t centerX = center.x();
    int32_t centerY = center.y();
    int32_t radius = size / 2 - 5;

    float angle = (value() - minimum()) / float(maximum() - minimum());
    angle *= 360.0;

    painter.save();
    painter.translate(center);
    painter.rotate(angle);
    painter.translate(-center);
    painter.setPen(QPen(Colors::cyan, 3));
    painter.setBrush(Colors::dark_gray);
    painter.drawEllipse(center, size / 2 - 3, size / 2 - 3);

    painter.setPen(QPen(Colors::cyan, 2));
    constexpr int8_t length = 18;
    QPoint top = QPoint(centerX, centerY - radius + length);
    QPoint bottom = QPoint(centerX, centerY + radius - length);
    painter.drawLine(top, bottom);

    QPolygonF pointer_arrow;
    pointer_arrow << QPointF(centerX, centerY - radius + 4)
                  << QPointF(centerX - 6, centerY - radius + 12)
                  << QPointF(centerX + 6, centerY - radius + 12);
    painter.drawPolygon(pointer_arrow);


    painter.restore();
}
void CRS_knob::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setSliderDown(true);

        QPoint delta = event->pos() - rect().center();
        m_last_angle = std::atan2(delta.x(), -delta.y()) * 180.0 / M_PI;
        if (m_last_angle < 0.0)
            m_last_angle += 360.0;
        event->accept();
    } else {
        QDial::mousePressEvent(event);
    }
}

void CRS_knob::mouseMoveEvent(QMouseEvent* event) {
    if (isSliderDown()) {
        QPoint center = rect().center();
        QPoint delta = event->pos() - center;

        double current_angle = std::atan2(delta.x(), -delta.y()) * 180.0 / M_PI;
        if (current_angle < 0.0)
            current_angle += 360.0;

        double angle_delta = current_angle - m_last_angle;

        if (angle_delta > 180.0) {
            angle_delta -= 360.0;
        } else if (angle_delta < -180.0) {
            angle_delta += 360.0;
        }

        int target_val = value() + static_cast<int>(angle_delta);
        target_val = ((target_val % 360) + 360) % 360;

        setValue(target_val);
        emit courseChanged(static_cast<int16_t>(target_val));

        m_last_angle = current_angle;
        event->accept();
    } else {
        QDial::mouseMoveEvent(event);
    }
}
void CRS_knob::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && isSliderDown()) {
        setSliderDown(false);
        event->accept();
    } else {
        QDial::mouseReleaseEvent(event);
    }
}
