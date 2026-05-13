#include "QNH_knob.hpp"
namespace Colors {
static const QColor cyan("#00a8e8");
static const QColor dark_gray("#1a1a1a");
} // namespace Colors

QNH_knob::QNH_knob(QWidget* parent) : QDial(parent) {
    setMinimumSize(40, 40);
    setRange(0, 360);
    setValue(0);
    setNotchesVisible(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void QNH_knob::paintEvent(QPaintEvent* event) {

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
    painter.restore();
}
void QNH_knob::mousePressEvent(QMouseEvent* event) {
    event->ignore();
}
