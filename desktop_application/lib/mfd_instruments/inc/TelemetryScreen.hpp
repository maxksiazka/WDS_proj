#ifndef TELEMETRY_SCREEN_HPP_
#define TELEMETRY_SCREEN_HPP_
#include <QFrame>
#include <QGridLayout>
#include <QLabel>

/**
 * @file TelemetryScreen.hpp
 * @brief Defines the TelemetryScreen class for displaying miscellaneous
 * telemetry data.
 *
 * The TelemetryScreen class is responsible for displaying various telemetry
 * data such as:
 *  - Connection status
 *  - MCU uptime
 *  - Barometric pressure
 *  - GPS fix status and satellite count
 *  - GPS coordinates
 *  - Ground speed and wind component
 *
 * It also includes a watchdog timer to detect loss of telemetry updates and
 * update the UI accordingly.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-07
 */

/**
 * @brief Struct representing the telemetry data to be displayed on the
 * TelemetryScreen.
 *
 * This is data that the class below depends on to update the UI.
 *
 */
struct TelemetryData {
    /**
     * @brief -- The timestamp of the telemetry data in microseconds since MCU
     * boot.
     */
    uint64_t timestamp_us;
    /**
     * @brief -- The barometric pressure reading in hPa.
     */
    float pressure_hpa;
    /**
     * @brief -- The GPS fix type (0 for no fix, 1 for 2D fix, 2 for 3D fix).
     */
    uint8_t gps_fix_type;
    /**
     * @brief -- The number of GPS satellites in view.
     */
    uint8_t gps_sats_count;
    /**
     * @brief -- The GPS latitude in degrees.
     */
    double gps_lat;
    /**
     * @brief -- The GPS longitude in degrees.
     */
    double gps_lon;
    /**
     * @brief -- The airspeed reading in knots.
     */
    float airspeed_kt;
    /**
     * @brief -- The ground speed reading in knots.
     */
    float ground_speed_kt;
    /**
     * @brief -- The acceleration readings in m/s^2 along the X, Y, and Z axes.
     */
    float accel_x;
    /**
     * @brief -- The acceleration readings in m/s^2 along the X, Y, and Z axes.
     */
    float accel_y;
    /**
     * @brief -- The acceleration readings in m/s^2 along the X, Y, and Z axes.
     */
    float accel_z;
};
class TelemetryScreen : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- QLabel pointers for displaying various telemetry data on the
     * UI.
     *
     * These labels are updated when new telemetry data is received to reflect
     * the latest information.
     */
    QLabel* m_lbl_uptime;
    /**
     * @brief -- QLabel for displaying the current barometric pressure in hPa.
     */
    QLabel* m_lbl_pressure;
    /**
     * @brief -- QLabel for displaying the GPS fix status (e.g., "NO FIX", "2D
     * FIX", "3D FIX") with color coding.
     */
    QLabel* m_lbl_gps_fix;
    /**
     * @brief -- QLabel for displaying the number of GPS satellites in view.
     */
    QLabel* m_lbl_gps_sats;
    /**
     * @brief -- QLabel for displaying the current GPS coordinates (latitude and
     * longitude).
     */
    QLabel* m_lbl_coords;
    /**
     * @brief -- QLabel for displaying the current connection status.
     */
    QLabel* m_lbl_conn_state;
    /**
     * @brief -- QLabel for displaying the current ground speed in knots.
     */
    QLabel* m_lbl_ground_speed;
    /**
     * @brief -- QLabel for displaying the current wind component
     * (HEADWIND/TAILWIND) in knots.
     */
    QLabel* m_lbl_wind_component;
    /**
     * @brief -- The square mean of the forces measured by the accelerometer,
     * calculated as sqrt(ax^2 + ay^2 + az^2), displayed in g's.
     */
    QLabel* m_lbl_gforce;
    /**
     * @brief -- QTimer used as a watchdog to detect loss of telemetry updates.
     */
    QTimer* m_watchdog_timer;

    /**
     * @brief Helper function to add a labeled row to the telemetry grid layout,
     * consisting of a static label and a value field that will be updated with
     * telemetry data.
     *
     * @param[in] grid -- The QGridLayout to which the row will be added.
     * @param[in] row -- The row index in the grid layout where the new row will
     * be added.
     * @param[in] label_text -- The text for the static label describing the
     * telemetry data (e.g., "UPTIME", "PRESSURE").
     * @param[in] value_label -- A pointer reference to a QLabel pointer that
     * will be initialized to point to the value label created for this row.
     * (meaning one of the pointers defined above).
     */
    void addTelemetryRow(QGridLayout* grid, int row, const QString& label_text,
                         QLabel*& value_label);

    /**
     * @brief Helper funtion to update the value label of uptime.
     *
     * @param[in] timestamp_us -- The newest timestamp in microseconds since MCU
     * boot.
     */
    void updateUptime(uint64_t timestamp_us);
    /**
     * @brief Helper function to update the value label of barometric pressure.
     *
     * @param[in] pressure_hpa -- The newest barometric pressure reading in hPa.
     */
    void updateBaroPressure(float pressure_hpa);
    /**
     * @brief Helper function to update the value labels related to GPS
     * diagnostics, including the GPS fix status, satellite count, and
     * coordinates.
     *
     * @param[in] fix_type -- The GPS fix type (0 for no fix, 1 for 2D fix, 2
     * for 3D fix).
     * @param[in] sats_count -- The number of GPS satellites in view.
     * @param[in] lat -- The latest GPS latitude in degrees.
     * @param[in] lon -- The latest GPS longitude in degrees.
     */
    void updateGNSSDiagnostics(uint8_t fix_type, uint8_t sats_count, double lat,
                               double lon);
    /**
     * @brief Helper function to update the value labels related to kinematics
     * and wind.
     *
     * @param[in] airspeed_kt -- The latest airspeed reading in knots.
     * @param[in] ground_speed_kt -- The latest ground speed reading in knots.
     */
    void updateKinematicsAndWind(float airspeed_kt, float ground_speed_kt);
    /**
     * @brief Helper function to update the value label of g-force/structural
     * load.
     *
     * @param[in] ax -- The latest acceleration reading along the X-axis in
     * m/s^2.
     * @param[in] ay -- The latest acceleration reading along the Y-axis in
     * m/s^2.
     * @param[in] az -- The latest acceleration reading along the Z-axis in
     * m/s^2.
     */
    void updateAccelerationVectors(float ax, float ay, float az);
    /**
     * @brief Helper function to set the UI to a "disconnected" state.
     *
     * Is triggered by watchdog timeout.
     */
    void setDisconnectedUI();
  private slots:
    /**
     * @brief Slot function that is called when the watchdog timer times out.
     *
     * This function will update the UI to reflect the disconnected state.
     *
     */
    void handleWatchdogTimeout();

  public:
    /**
     * @brief Constructor of the TelemetryScreen class, which initializes the
     * telemetry panel and sets up the UI components and watchdog timer.
     *
     * @param[in] parent -- The parent widget of the telemetry screen, default
     * is nullptr.
     */
    explicit TelemetryScreen(QWidget* parent = nullptr);
    ~TelemetryScreen() = default;

  public slots:
    /**
     * @brief API slot function to update the telemetry data displayed on the
     * screen when new data is received.
     *
     * @param[in] data -- The new telemetry data received, as defined in the
     * \link TelemetryData \endlink struct.
     */
    void updateTelemetryData(const TelemetryData& data);
};
#endif // TELEMETRY_SCREEN_HPP_
