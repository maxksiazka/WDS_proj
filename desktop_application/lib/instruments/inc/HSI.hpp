#ifndef HSI_H_
#define HSI_H_
#include <QFrame>

class HSI : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief m_current_heading -- The current heading of the aircraft in degrees.
     */
    double m_current_heading;
    /**
     * @brief m_current_crs -- The current course of the aircraft in degrees.
     */
    int16_t m_current_crs;

    /**
     * @brief Event handler for the paint event. This function is responsible for drawing the HSI widget based on the current heading and course of the aircraft.
     *
     * @param[in] event -- The paint event that triggered this function.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    explicit HSI(QWidget* parent = nullptr);
  public slots:
    /**
     * @brief Slot used to update the current heading of the aircraft. This will trigger a repaint of the widget to reflect the new heading.
     *
     * @param[in] heading -- The new heading of the aircraft in degrees.
     */
    void setCurrentHeading(double heading);
    /**
     * @brief Slot used to update the current course of the aircraft. This will trigger a repaint of the widget to reflect the new course.
     *
     * @param[in] crs -- The new course of the aircraft in degrees.
     */
    void setCurrentCRS(int16_t crs);
};
#endif // HSI_H_
