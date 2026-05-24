#ifndef DCP_HPP_
#define DCP_HPP_

#include "CRS_knob.hpp"
#include "DataFusionEngine.hpp"
#include "QNH_knob.hpp"
#include <QDial>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

class DCP : public QFrame {
    Q_OBJECT
  private:
    QHBoxLayout* main_layout_;

    QNH_knob* qnh_knob_;
    QLabel* qnh_label_;

    QPushButton* btn_nav_;
    QPushButton* btn_autopilot_;
    QPushButton* btn_settings_;

    CRS_knob* crs_knob_;
    QLabel* crs_label_;

    /**
     * @brief Internal method to set up the UI components of the DCP panel.
     *
     * This method initializes and arranges the widgets (QNH knob, buttons, CRS
     * knob) within the DCP panel using appropriate layouts. It also configures
     * the properties of the widgets, such as size policies and font settings.
     */
    void setupUI();
    /**
     * @brief Internal method to set up the stylesheet for the DCP panel.
     *
     */
    void setupStylesheet();

  public:
    /**
     * @brief DCP constructor.
     *
     * @param[in] parent -- The parent widget of the DCP panel, default is
     * nullptr.
     */
    explicit DCP(QWidget* parent = nullptr);
    void connectDataFusionEngineToQNHKnob(DataFusionEngine* engine);
};

#endif // DCP_HPP_
