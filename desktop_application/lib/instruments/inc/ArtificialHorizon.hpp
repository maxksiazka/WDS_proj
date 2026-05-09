#ifndef ARTIFICIALHORIZON_HPP_
#define ARTIFICIALHORIZON_HPP_

#include <Eigen/Geometry>
#include <QFrame>
#include <QPainter>

class ArtificialHorizon : public QFrame {
    Q_OBJECT
  private:
    double pitch_ = 0.0;
    double roll_ = 0.0;
    void paintEvent(QPaintEvent* event) override;

  public:
    explicit ArtificialHorizon(QWidget* parent = nullptr);
    ~ArtificialHorizon() = default;
  public slots:
    void updateOrientation(const Eigen::Quaterniond& orientation);
};

#endif // ARTIFICIALHORIZON_HPP_
