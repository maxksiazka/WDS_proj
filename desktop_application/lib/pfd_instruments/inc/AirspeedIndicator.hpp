#ifndef AIRSPEEDINDICATOR_HPP_
#define AIRSPEEDINDICATOR_HPP_
/**
 * @file AirspeedIndicator.hpp
 * @brief Defines the AirspeedIndicator class for displaying the current airspeed
 * of the aircraft in knots.
 * @author Maksymilian Ksiazka
 * @date 2026-05-13
 */

#include <Eigen/Geometry>
#include <QFrame>

/**
 * @brief AirspeedIndicator widget for displaying the current airspeed of the
 * aircraft in knots.
 *
 * The widget is a vertical tape indicator, similar to the airspeed tape
 * on a real PFD. It obtains the current airspeed value from the
 * DataFusionEngine and updates its display accordingly. The airspeed is
 * displayed in knots.
 */
class AirspeedIndicator : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- The current airspeed in knots, updated from the
     * DataFusionEngine.
     */
    double m_current_airspeed;

    /**
     * @brief Custom paint event.
     *
     * Draws the airspeed indicator based on the current airspeed value.
     *
     * @param[in] event -- The paint event containing information about the area
     * to be repainted.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    /**
     * @brief Constructs a new AirspeedIndicator widget.
     *
     * @param[in] parent -- Pointer to the parent widget, defaulting to nullptr.
     */
    explicit AirspeedIndicator(QWidget* parent = nullptr);
    /**
     * @brief Default destructor.
     *
     */
    ~AirspeedIndicator() = default;
  public slots:
    /**
     * @brief Changes the displayed airspeed value.
     *
     * This method is typically connected to a signal emitted by the
     * DataFusionEngine whenever the airspeed estimate is updated.
     *
     * @param[in] airspeed -- The new airspeed value in knots to be displayed on
     * the indicator.
     */
    void updateAirspeed(double airspeed);
};
#endif // AIRSPEEDINDICATOR_HPP_
