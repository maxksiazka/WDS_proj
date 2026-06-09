#ifndef MFD_HPP_
#define MFD_HPP_

#include "DataFusionEngine.hpp"
#include "NavigationScreen.hpp"
#include "SensorLink.hpp"
#include "TelemetryScreen.hpp"
#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>
#include <mutex>

class MFD : public QFrame {
    Q_OBJECT
  private:
    std::mutex FPLN_mutex_;
    /**
     * @brief -- Pointer to the telemetry panel, which displays miscellaneous
     * telemetry data.
     *
     * Displays:
     *  - Connection status
     *  - MCU uptime
     *  - Barometric pressure
     *  - GPS fix status and satellite count
     *  - GPS coordinates
     *  - Ground speed and wind component
     */
    TelemetryScreen* m_telemetry_panel;

    /**
     * @brief -- Pointer to the navigation map panel.
     * Currently a placeholder QFrame, to be switched for a dedicated class
     * later.
     */
    NavigationScreen* m_nav_map_panel;
    /**
     * @brief Helper function to initialize the test flight plan on the
     * navigation map panel.
     */
    void initializeTestFlightPlan();
  private slots:
    /**
     * @brief Is called when new sensor data is received from by SensorLink.
     *
     * @param[in] data -- The new sensor data received from the SensorLink, as
     * defined in \link SensorData \endlink struct.
     */
    void sensorDataUpdated(const SensorData& data);
    /**
     * @brief Is to be called when a button forces a flight plan update on the
     * navigation map panel.
     *
     */
    void FPLNForcedUpdate();

    /**
     * @brief Is to be called when a button forces a flight plan clear on the
     * navigation map panel.
     *
     */
    void FPLNForcedClear();
    /**
     * @brief Is to be called when a button forces a zoom level change on the
     * navigation map panel.
     */
    void ZoomLevelChanged();

  signals:
    /**
     * @brief Signal emitted when new telemetry data is available after
     * processing the raw sensor data.
     *
     * @param[in] data -- The updated telemetry data, as defined in \link
     * TelemetryData \endlink struct.
     */
    void telemetryDataUpdated(const TelemetryData& data);

  public:
    /**
     * @brief MFD constructor.
     * @param[in] parent -- The parent widget of the MFD panel, default is
     * nullptr.
     */
    explicit MFD(QWidget* parent = nullptr);
    /**
     * @brief API function to connect the MFD to SensorLink, to be called from
     * main.
     *
     * @param[in] link -- Pointer to the SensorLink instance.
     */
    void connectToSensorLink(SensorLink*);
    /**
     * @brief API function to connect the MFD to the DataFusionEngine, to be
     * called from \link MainWindow \endlink constructor.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance.
     */
    void connectToDataFusionEngine(DataFusionEngine* engine);
    ~MFD() = default;
};

#endif // MFD_HPP_
