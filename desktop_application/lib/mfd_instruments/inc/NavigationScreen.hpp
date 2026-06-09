#ifndef NAVIGATION_SCREEN_HPP_
#define NAVIGATION_SCREEN_HPP_

#include <QList>
#include <QString>
#include <QFrame>
#include <QWidget>

struct Waypoint {
    QString name;
    double lat;
    double lon;
};

class NavigationScreen : public QFrame {
    Q_OBJECT
  private:
    double m_planeLat = 0.0;
    double m_planeLon = 0.0;
    double m_planeHeading = 0.0;
    double m_scale = 3000.0; // Default zoom
    QList<Waypoint> m_waypoints;
    void paintEvent(QPaintEvent* event) override;
  public:
    explicit NavigationScreen(QWidget* parent = nullptr);
    void addWaypoint(const QString& name, double lat, double lon);
    void clearWaypoints();
    void setZoom(double scale);
  public slots:
    void setPlanePosition(double lat, double lon, double heading);
};

#endif // NAVIGATION_SCREEN_HPP_
