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
     * Set to ignore -- only dragging the knob should change its value, not
     * clicking on it.
     *
     * @param event -- The mouse event containing information about the mouse
     * press action.
     */
    void mousePressEvent(QMouseEvent* event) override;

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
