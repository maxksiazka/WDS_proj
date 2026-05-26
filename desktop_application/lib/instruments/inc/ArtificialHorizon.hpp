#ifndef ARTIFICIALHORIZON_HPP_
#define ARTIFICIALHORIZON_HPP_

#include <Eigen/Geometry>
#include <QFrame>
#include <QPainter>

/**
 * @brief ArtificialHorizon widget for displaying the attitude of the aircraft,
 * including pitch and roll.
 *
 * This widget is the primary component of the PFD, mimicking AHRS systems used
 * in real aircraft. It uses data obtained from the DataFusionEngine to render a
 * visual representation of orientation.
 */
class ArtificialHorizon : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- The current pitch angle in degrees, updated from the
     * DataFusionEngine.
     */
    double m_pitch = 0.0;
    /**
     * @brief -- The current roll angle in degrees, updated from the
     * DataFusionEngine.
     */
    double m_roll = 0.0;
    /**
     * @brief Custom paint event handler.
     *
     * Draws the artificial horizon based on the current pitch and roll values.
     *
     * @param[in] event -- The paint event triggered.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    /**
     * @brief Constructs a new ArtificialHorizon widget.
     *
     * @param[in] parent -- The parent widget of the artificial horizon, default
     * is nullptr.
     */
    explicit ArtificialHorizon(QWidget* parent = nullptr);
    /**
     * @brief Default destructor.
     */
    ~ArtificialHorizon() = default;
  public slots:
    /**
     * @brief Updates the orientation of the artificial horizon based on the
     * provided quaternion.
     *
     * @param[in] orientation -- The new orientation of the aircraft represented
     * as a quaternion.
     */
    void updateOrientation(const Eigen::Quaterniond& orientation);
};

#endif // ARTIFICIALHORIZON_HPP_
