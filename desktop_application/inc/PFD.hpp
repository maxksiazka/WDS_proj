#ifndef PFD_HPP_
#define PFD_HPP_

#include "AirspeedIndicator.hpp"
#include "ArtificialHorizon.hpp"
#include "Altimeter.hpp"
#include "CRS_knob.hpp"
#include "HSI.hpp"
#include "QNH_knob.hpp"
#include "SystemStatusHeader.hpp"
#include "DataFusionEngine.hpp"
#include <QFrame>
#include <QGridLayout>
#include <QWidget>

class PFD : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- Pointer to the ArtificialHorizon widget, which displays the
     * attitude of the aircraft.
     */
    ArtificialHorizon* m_artificial_horizon;
    /**
     * @brief -- Pointer to the AirspeedIndicator widget, which displays the
     * current airspeed of the aircraft.
     */
    AirspeedIndicator* m_airspeed_indicator;
    /**
     * @brief -- Pointer to the Altimeter widget, which displays the current
     * altitude of the aircraft.
     */
    Altimeter* m_altimeter;
    /**
     * @brief -- Pointer to the HSI widget, which displays the heading and
     * navigation information.
     */
    HSI* m_hsi;
    /**
     * @brief -- Pointer to the SystemStatusHeader widget, which displays
     * system status information such as GPS satellite count and fix status.
     */
    SystemStatusHeader* m_status_header;
    /**
     * @brief Internal method to set up the left panel of the PFD.
     *
     * Sets up the left panel of the PFD, which includes the artificial horizon,
     * airspeed indicator, and IAS and altitude tapes.  
     * @param[in] grid -- The grid layout to which the left panel components will be added.
     */
    void setupLeftPanel(QGridLayout* grid);

  public:
    /**
     * @brief Method to connect a SensorLink instance to the PFD components.
     *
     * @param[in] link -- Pointer to the SensorLink instance that will provide sensor data.
     */
    void connectSensorLinkToPFD(SensorLink* link);
    /**
     * @brief Method to connect a DataFusionEngine instance to the PFD components.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will provide fused sensor data for display on the PFD.
     */
    void connectDataFusionEngineToPFDComponents(DataFusionEngine* engine);
    /**
     * @brief Method to connect the DCP components (CRS knob and QNH knob) to the PFD, allowing the PFD to receive updates when the knobs are adjusted.
     *
     * @param[in] crs_knob -- Pointer to the CRS_knob instance from the DCP, which will allow the PFD to receive updates when the course setting is adjusted.
     * @param[in] qnh_knob -- Pointer to the QNH_knob instance from the DCP, which will allow the PFD to receive updates when the barometric pressure setting is adjusted.
     */
    void connectDCPToPFD(CRS_knob* crs_knob, QNH_knob* qnh_knob);
    /**
     * @brief PFD constructor.
     *
     * @param[in] parent -- The parent widget of the PFD panel, default is nullptr.
     */
    explicit PFD(QWidget* parent = nullptr);
    ~PFD() = default;
};

#endif // PFD_HPP_
