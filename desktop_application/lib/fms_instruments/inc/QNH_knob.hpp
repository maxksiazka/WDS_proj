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

class QNH_knob : public QDial {
    Q_OBJECT

  private:
    double last_angle_ = 0.0;
    /**
     * @brief Custom paint event to draw the QNH knob with specific design.
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
     * @brief Custom mouse press event to handle interactions with the QNH knob.
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
     * QNH knob.
     *
     * This method overrides the default mouse move event to calculate the angle
     * of the mouse movement relative to the center of the knob and update the
     * knob's value accordingly. It also emits a signal with the new QNH value
     * in hPa whenever the knob is dragged.
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
