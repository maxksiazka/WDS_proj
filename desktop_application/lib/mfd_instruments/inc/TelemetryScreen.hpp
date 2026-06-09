#ifndef TELEMETRY_SCREEN_HPP_
#define TELEMETRY_SCREEN_HPP_
#include <QFrame>
#include <QGridLayout>
#include <QLabel>


struct TelemetryData {
    uint64_t timestamp_us;
    float pressure_hpa;
    uint8_t gps_fix_type;
    uint8_t gps_sats_count;
    double gps_lat;
    double gps_lon;
    float airspeed_kt;
    float ground_speed_kt;
    float accel_x;
    float accel_y;
    float accel_z;
};
class TelemetryScreen : public QFrame {
    Q_OBJECT
  private:
    QLabel* m_lbl_uptime;
    QLabel* m_lbl_pressure;
    QLabel* m_lbl_gps_fix;
    QLabel* m_lbl_gps_sats;
    QLabel* m_lbl_coords;
    QLabel* m_lbl_conn_state;
    QLabel* m_lbl_ground_speed;
    QLabel* m_lbl_wind_component;
    QLabel* m_lbl_gforce;
    QTimer* m_watchdog_timer;

    void addTelemetryRow(QGridLayout* grid, int row, const QString& label_text,
                         QLabel*& value_label);
    void updateUptime(uint64_t timestamp_us);
    void updateBaroPressure(float pressure_hpa);
    void updateGNSSDiagnostics(uint8_t fix_type, uint8_t sats_count, double lat,
                               double lon);
    void updateKinematicsAndWind(float airspeed_kt, float ground_speed_kt);
    void updateAccelerationVectors(float ax, float ay, float az);
    void setDisconnectedUI();
  private slots:
    void handleWatchdogTimeout();

  public:
    explicit TelemetryScreen(QWidget* parent = nullptr);
    ~TelemetryScreen() = default;

  public slots:
    void updateTelemetryData(const TelemetryData& data);
};
#endif // TELEMETRY_SCREEN_HPP_
