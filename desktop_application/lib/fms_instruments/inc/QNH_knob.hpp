#ifndef QNH_KNOB_HPP_
#define QNH_KNOB_HPP_
/**
 * @file QNH_knob.hpp
 * @brief Custom QDial widget representing the QNH knob in the FMS panel.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-13
 */
#include <QDial>
#include <QMouseEvent>
#include <QPainter>

/**
 * @brief QNH_knob is a custom widget that inherits from QDial, representing the QNH knob in the display control panel (DCP).
 *
 * The QNH knob allows the user to adjust the barometric pressure setting (QNH) by dragging the knob.
 */
class QNH_knob : public QDial {
    Q_OBJECT

  private:
    /**
     * @brief -- Last saved angle of the knob in degrees, used to calculate the change in angle during dragging.
     */
    double m_last_angle = 0.0;
    /**
     * @brief Custom paint event.
     *
     * Draws the QNH knob, based on the current angle of the knob.
     *
     * @param event -- The paint event containing information about the area to
     * be repainted.
     */
    void paintEvent(QPaintEvent* event) override;
    /**
     * @brief Custom mouse press event to handle interactions with the QNH knob.
     *
     * Only dragging the knob changes its value, not clicking on it.
     *
     * @param event -- The mouse event containing information about the mouse
     * press action.
     */
    void mousePressEvent(QMouseEvent* event) override;
    /**
     * @brief Custom mouse move event to handle dragging interactions with the
     * QNH knob.
     *
     * This method overrides the default mouse move event. 
     * The point is to disable clicking changing the value.
     *
     * @param[in] event -- The mouse event containing information about the
     * mouse move action.
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    /**
     * @brief Custom mouse release event to handle the end of dragging
     * interactions with the QNH knob.
     *
     * This releases the slider when LMB is released.
     *
     * @param[in] event -- The mouse event containing information about the
     * mouse release action.
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
  signals:
    /**
     * @brief Emitted when the QNH value is changed by dragging the knob.
     *
     * @param[in] qnh_hPa -- The new QNH value in hPa, calculated based on the knob's position.
     */
    void qnhChanged(double qnh_hPa);

  public:
    /**
     * @brief QNH_knob constructor.
     *
     * @param[in] parent -- The parent widget of the QNH knob, default is
     * nullptr.
     */
    explicit QNH_knob(QWidget* parent = nullptr);
};

#endif // QNH_KNOB_HPP_
