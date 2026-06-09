#include "MFD.hpp"
#include <QLabel>
#include <QVector>
MFD::MFD(QWidget* parent) : QFrame(parent) {
    setStyleSheet("QFrame { background-color: #0a0a0a; border-radius: 2px; "
                  "border: 1px solid #333; }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);
    m_telemetry_panel = new TelemetryScreen(this);
    m_nav_map_panel = new NavigationScreen(this);
    layout->addWidget(m_telemetry_panel, 1);
    layout->addWidget(m_nav_map_panel, 2);
    initializeTestFlightPlan();
    connect(this, SIGNAL(telemetryDataUpdated(const TelemetryData&)),
            m_telemetry_panel, SLOT(updateTelemetryData(const TelemetryData&)));
}
void MFD::sensorDataUpdated(const SensorData& data) {
    TelemetryData telemetry_data;
    telemetry_data.timestamp_us = data.timestamp_us;
    telemetry_data.pressure_hpa = data.pressure;
    telemetry_data.gps_fix_type = data.gps_fix;
    telemetry_data.gps_sats_count = data.gps_sats;
    telemetry_data.gps_lat = data.gps_lat;
    telemetry_data.gps_lon = data.gps_lon;
    telemetry_data.airspeed_kt = data.airspeed;
    telemetry_data.ground_speed_kt = data.gps_ground_speed;
    telemetry_data.accel_x = data.accel[0];
    telemetry_data.accel_y = data.accel[1];
    telemetry_data.accel_z = data.accel[2];

    emit telemetryDataUpdated(telemetry_data);
}
void MFD::connectToSensorLink(SensorLink* link) {
    connect(link, SIGNAL(data_received(const SensorData&)), this,
            SLOT(sensorDataUpdated(const SensorData&)));
}
// Inside MFD initialization or a custom loading slot:
void MFD::initializeTestFlightPlan() {
    m_nav_map_panel->addWaypoint("ALPHA", 51.1, 17.0);
    m_nav_map_panel->addWaypoint("BRAVO", 51.2, 17.2);
}
void MFD::connectToDataFusionEngine(DataFusionEngine* engine) {
    connect(engine, SIGNAL(GPSHeadingUpdated(double, double, double)),
            m_nav_map_panel,
            SLOT(setPlanePosition(double, double, double)));
}
