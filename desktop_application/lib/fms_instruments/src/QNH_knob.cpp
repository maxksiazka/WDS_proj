#include "QNH_knob.hpp"
#include <algorithm>
#include <cmath>

namespace Colors {
static const QColor cyan("#00a8e8");
static const QColor dark_gray("#1a1a1a");
} // namespace Colors

static constexpr double UNITS_PER_ROTATION = 2000.0;

QNH_knob::QNH_knob(QWidget* parent) : QDial(parent) {
    setMinimumSize(40, 40);
    setRange(95000, 105000);
    setValue(101325);

    setSingleStep(10);
    setPageStep(100);
    setNotchesVisible(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    connect(this, &QDial::valueChanged, this, [this](int val) {
        qDebug() << "QNH value changed:" << val;
        emit qnhChanged(val / 100.0);
    });
}

void QNH_knob::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setSliderDown(true);

        QPoint delta = event->pos() - rect().center();
        last_angle_ = std::atan2(delta.x(), -delta.y()) * 180.0 / M_PI;
        if (last_angle_ < 0.0)
            last_angle_ += 360.0;
        event->accept();
    } else {
        QDial::mousePressEvent(event);
    }
}

void QNH_knob::mouseMoveEvent(QMouseEvent* event) {
    if (isSliderDown()) {
        QPoint center = rect().center();
        QPoint delta = event->pos() - center;

        double current_angle = std::atan2(delta.x(), -delta.y()) * 180.0 / M_PI;
        if (current_angle < 0.0)
            current_angle += 360.0;

        double angle_delta = current_angle - last_angle_;

        if (angle_delta > 180.0) {
            angle_delta -= 360.0;
        } else if (angle_delta < -180.0) {
            angle_delta += 360.0;
        }

        double value_delta = (angle_delta / 360.0) * UNITS_PER_ROTATION;

        int target_val = value() + static_cast<int>(value_delta);
        target_val = std::clamp(target_val, minimum(), maximum());

        setValue(target_val);

        last_angle_ = current_angle;
        event->accept();
    } else {
        QDial::mouseMoveEvent(event);
    }
}

void QNH_knob::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && isSliderDown()) {
        setSliderDown(false);
        event->accept();
    } else {
        QDial::mouseReleaseEvent(event);
    }
}

void QNH_knob::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int32_t size = width();
    QRect rect(0, 0, size, size);
    QPoint center = rect.center();
    int32_t centerX = center.x();
    int32_t centerY = center.y();
    int32_t radius = size / 2 - 5;

    float angle = std::fmod(static_cast<double>(value()), UNITS_PER_ROTATION) /
                  UNITS_PER_ROTATION * 360.0;

    painter.translate(center);
    painter.rotate(angle);
    painter.translate(-center);
    painter.setPen(QPen(Colors::cyan, 3));
    painter.setBrush(Colors::dark_gray);
    painter.drawEllipse(center, size / 2 - 3, size / 2 - 3);

    QFont font("Segoe UI", 8, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Colors::cyan);
    QString topText = "PUSH";
    QFontMetrics fm(font);
    int text_width = fm.horizontalAdvance(topText);
    painter.drawText(center.x() - text_width / 2, center.y() - radius + 12,
                     topText);

    QString bottomText = "STD";
    text_width = fm.horizontalAdvance(bottomText);
    painter.drawText(center.x() - text_width / 2, center.y() + radius - 4,
                     bottomText);

    painter.setPen(QPen(Colors::cyan, 2));
    constexpr int8_t length = 18;
    QPoint top = QPoint(centerX, centerY - radius + length);
    QPoint bottom = QPoint(centerX, centerY + radius - length);
    painter.drawLine(top, bottom);
}
