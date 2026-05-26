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

/**
 * @brief DCP (Display Control Panel) widget for the virtual cockpit simulator.
 *
 * The DCP is a horizontal panel located at the top of the main window. Its main
 * purpose is to provide controls for adjusting QNH and CRS.
 */
class DCP : public QFrame {
    Q_OBJECT
  private:
    QHBoxLayout* main_layout_;

    /**
     * @brief -- QNH knob custom widget for adjusting the barometric pressure
     * setting.
     */
    QNH_knob* qnh_knob_;
    /**
     * @brief -- QLabel to display the current QNH value next to the QNH knob.
     */
    QLabel* qnh_label_;

    /**
     * @brief -- QPushButton widget, currently unused
     */
    QPushButton* btn_nav_;
    /**
     * @brief -- QPushButton for 'autopilot', currently unused
     */
    QPushButton* btn_autopilot_;
    /**
     * @brief -- QPushButton for 'settings', currently unused
     */
    QPushButton* btn_settings_;

    /**
     * @brief -- CRS knob custom widget for adjusting the course setting.
     */
    CRS_knob* crs_knob_;
    /**
     * @brief -- QLabel to display the current CRS value next to the CRS knob.
     */
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
    /**
     * @brief Simple getter method to access the CRS knob widget from outside
     * the DCP class.
     */
    CRS_knob* getCRS() const;

    /**
     * @brief Simple getter method to access the QNH knob widget from outside
     * the DCP class.
     */
    QNH_knob* getQNH() const;
    /**
     * @brief Method to connect the signals from the QNH knob to the appropriate
     * slots in the DataFusionEngine.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will
     * receive the signals from QNH knob changes.
     */
    void connectDataFusionEngineToQNHKnob(DataFusionEngine* engine);
};

#endif // DCP_HPP_
