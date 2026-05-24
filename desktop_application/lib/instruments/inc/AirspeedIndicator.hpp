#ifndef AIRSPEEDINDICATOR_HPP_
#define AIRSPEEDINDICATOR_HPP_

#include <Eigen/Geometry>
#include <QFrame>

class AirspeedIndicator : public QFrame {
    Q_OBJECT
  private:
    double m_current_airspeed;

    void paintEvent(QPaintEvent* event) override;
  public:
    explicit AirspeedIndicator(QWidget* parent = nullptr);
    ~AirspeedIndicator() = default;
  public slots:
    void updateAirspeed(double airspeed);
};
#endif // AIRSPEEDINDICATOR_HPP_
