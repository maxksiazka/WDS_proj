#ifndef CRS_KNOB_HPP_
#define CRS_KNOB_HPP_
/**
 * @file CRS_knob.hpp
 * @brief Custom QDial widget representing the CRS knob in the display control
 * panel.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-13
 */
#include <QDial>
#include <QMouseEvent>
#include <QPainter>

class CRS_knob : public QDial {
    Q_OBJECT
  private:
      double m_last_angle = 0.0;
    /**
     * @brief Custom paint event to draw the CRS knob with specific design.
     *
     * This method overrides the default paint event to render a circular knob
     * with a cyan outline and dark gray fill. It also draws "PUSH" at the top
     * and "STD" at the bottom of the knob, along with a line indicating the
     * current position.
     *
     * @param event -- The paint event containing information about the area to
     * be repainted.
     */
    void paintEvent(QPaintEvent* event) override;
    /**
     * @brief Custom mouse press event to handle interactions with the CRS knob.
     *
     * Only dragging the knob changes its value, not clicking on it. This method
     * overrides the default mouse press event to ignore clicks and only respond
     * to dragging actions.
     *
     * @param event -- The mouse event containing information about the mouse
     * press action.
     */
    void mousePressEvent(QMouseEvent* event) override;
    /**
     * @brief Custom mouse move event to handle dragging interactions with the
     * CRS knob.
     *
     * This method overrides the default mouse move event to calculate the angle
     * of the mouse movement relative to the center of the knob and update the
     * knob's value accordingly. It also emits a signal with the new CRS value
     * in hPa whenever the knob is dragged.
     *
     * @param[in] event -- The mouse event containing information about the
     * mouse move action.
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    /**
     * @brief Custom mouse release event to handle the end of dragging
     * interactions with the CRS knob.
     *
     * This releases the slider when LMB is released.
     *
     * @param[in] event -- The mouse event containing information about the
     * mouse release action.
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
  signals:
    /**
     * @brief Signal emitted when the course value of the knob changes.
     *
     * This signal is emitted whenever the user drags the knob to a new
     * position, indicating a change in the course value. The new course value
     * is passed as a parameter to the signal.
     *
     * @param newCourse -- The new course value in degrees after the change. It
     * can only assume values between 0 and 360 degrees, representing the full
     * range of possible courses.
     */
    void courseChanged(int16_t newCourse);

  public:
    /**
     * @brief CRS_knob constructor.
     *
     * @param[in] parent -- The parent widget of the CRS knob, default is
     * nullptr.
     */
    explicit CRS_knob(QWidget* parent = nullptr);
};

#endif // CRS_KNOB_HPP_
