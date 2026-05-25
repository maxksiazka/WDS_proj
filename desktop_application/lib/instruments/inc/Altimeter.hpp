#ifndef ALTIMETER_HPP_
#define ALTIMETER_HPP_

#include <Eigen/Geometry>
#include <QFrame>

class Altimeter : public QFrame {
    Q_OBJECT
  private:
    double m_current_altitude;
    void paintEvent(QPaintEvent* event) override;
  public:
    explicit Altimeter(QWidget* parent = nullptr);
    ~Altimeter() = default;
  public slots:
    void updateAltitude(double altitude);
};
#endif // ALTIMETER_HPP_
