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
    /**
     * @brief Custom paint event handler to draw the artificial horizon based on
     * the current pitch and roll values.
     *
     * This method overrides the default paint event of the QWidget to render
     * the artificial horizon. It uses QPainter to draw the horizon line, sky,
     * and ground based on the current orientation of the aircraft.
     *
     * @param[in] event -- The paint event triggered when the widget needs to be
     * redrawn.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    /**
     * @brief Constructs a new ArtificialHorizon widget.
     *
     * @param[in] parent -- The parent widget of the artificial horizon, default is nullptr.
     */
    explicit ArtificialHorizon(QWidget* parent = nullptr);
    ~ArtificialHorizon() = default;
  public slots:
    /**
     * @brief Updates the orientation of the artificial horizon based on the provided quaternion.
     *
     * @param[in] orientation -- The new orientation of the aircraft represented as a quaternion.
     */
    void updateOrientation(const Eigen::Quaterniond& orientation);
};

#endif // ARTIFICIALHORIZON_HPP_
