
#include "NavigationScreen.hpp"
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

NavigationScreen::NavigationScreen(QWidget* parent)
    : QFrame(parent), m_scale(3000.0) {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
    setMinimumSize(600, 400);
}

void NavigationScreen::setPlanePosition(double lat, double lon,
                                        double heading) {
    qDebug() << "Updating plane position: Lat=" << lat << " Lon=" << lon
             << " Heading=" << heading;
    m_planeLat = lat;
    m_planeLon = lon;
    m_planeHeading = heading;
    update();
}

void NavigationScreen::addWaypoint(const QString& name, double lat,
                                   double lon) {
    m_waypoints.append({name, lat, lon});
    update();
}

void NavigationScreen::clearWaypoints() {
    m_waypoints.clear();
    update();
}

void NavigationScreen::setZoom(double scale) {
    m_scale = scale;
    update();
}

void NavigationScreen::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int32_t width = this->width();
    const int32_t height = this->height();

    painter.save();

    painter.translate(width / 2.0, height / 2.0);

    painter.rotate(-m_planeHeading);

    painter.setPen(QPen(Qt::green, 2));
    painter.setBrush(Qt::NoBrush);

    const double cosLat = std::cos(m_planeLat * M_PI / 180.0);

    for (const Waypoint& wp : m_waypoints) {
        const double dLon = wp.lon - m_planeLon;
        const double dLat = wp.lat - m_planeLat;
        const double x = dLon * cosLat * m_scale;
        const double y = -dLat * m_scale;

        painter.drawEllipse(QPointF(x, y), 6, 6);

        painter.save();
        painter.translate(x, y);
        painter.rotate(m_planeHeading);
        painter.drawText(10, 5, wp.name);
        painter.restore();
    }
    painter.restore();
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Qt::white);

    painter.translate(width / 2.0, height / 2.0);
    QPainterPath airplane;
    airplane.moveTo(0, -12);
    airplane.lineTo(0, 12);
    airplane.moveTo(-18, -2);
    airplane.lineTo(18, -2);
    airplane.moveTo(-6, 8);
    airplane.lineTo(6, 8);
    painter.drawPath(airplane);
}
