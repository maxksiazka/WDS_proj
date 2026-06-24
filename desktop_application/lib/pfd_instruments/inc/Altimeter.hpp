#ifndef ALTIMETER_HPP_
#define ALTIMETER_HPP_
/**
 * @file Altimeter.hpp
 * @brief Defines the Altimeter class for displaying the current altitude of the
 * aircraft in feet.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-25
 */

#include <Eigen/Geometry>
#include <QFrame>

/**
 * @brief Altimeter widget for displaying the current altitude of the aircraft
 * in feet.
 *
 * The Altimeter is a vertical tape indicator, similar to the altitude
 * tape on a real PFD. It obtains the current altitude value from
 * DataFusionEngine and updates its display accordingly. The altitude is
 * displayed in feet.
 */
class Altimeter : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- The current altitude in feet, updated from the
     * DataFusionEngine.
     */
    double m_current_altitude;
    /**
     * @brief Custom paint event to draw the altimeter.
     *
     * Draws the altimeter based on the current altitude value.
     *
     * @param[in] event -- The paint event containing information about the area
     * to be repainted.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    /**
     * @brief Constructs a new Altimeter widget.
     *
     * @param[in] parent -- Pointer to the parent widget, defaulting to nullptr.
     * The Altimeter should be a child widget within the PFD.
     */
    explicit Altimeter(QWidget* parent = nullptr);
    /**
     * @brief Default destructor.
     */
    ~Altimeter() = default;
  public slots:
    /**
     * @brief Updates the displayed altitude value.
     *
     * This method is typically connected to a signal emitted by the
     * DataFusionEngine whenever the altitude estimate is updated.
     *
     * @param[in] altitude -- The new altitude value in feet to be displayed on
     * the altimeter.
     */
    void updateAltitude(double altitude);
};
#endif // ALTIMETER_HPP_
