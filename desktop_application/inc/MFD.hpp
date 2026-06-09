#ifndef MFD_HPP_
#define MFD_HPP_

#include "DataFusionEngine.hpp"
#include "NavigationScreen.hpp"
#include "SensorLink.hpp"
#include "TelemetryScreen.hpp"
#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

class MFD : public QFrame {
    Q_OBJECT
  private:
    TelemetryScreen* m_telemetry_panel;

    /**
     * @brief -- Pointer to the navigation map panel.
     * Currently a placeholder QFrame, to be switched for a dedicated class later.
     */
    NavigationScreen* m_nav_map_panel;
    void initializeTestFlightPlan();
  private slots:
    void sensorDataUpdated(const SensorData& data);

  signals:
    void telemetryDataUpdated(const TelemetryData& data);

  public:
    /**
     * @brief MFD constructor.
     * @param[in] parent -- The parent widget of the MFD panel, default is nullptr.
     */
    explicit MFD(QWidget* parent = nullptr);
    void connectToSensorLink(SensorLink*);
    void connectToDataFusionEngine(DataFusionEngine* engine);
    ~MFD() = default;
};

#endif // MFD_HPP_
