#include "MFD.hpp"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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
    m_nav_map_panel->addWaypoint({"ALPHA", 51.1, 17.0});
    m_nav_map_panel->addWaypoint({"BRAVO", 51.2, 17.2});
}
void MFD::connectToDataFusionEngine(DataFusionEngine* engine) {
    connect(engine, SIGNAL(GPSHeadingUpdated(double, double, double)),
            m_nav_map_panel, SLOT(setPlanePosition(double, double, double)));
}
void MFD::FPLNForcedUpdate() {
    std::lock_guard<std::mutex> lock(FPLN_mutex_);
    QString filename = QFileDialog::getOpenFileName(this, "Open Flight Plan",
                                                    "", "JSON Files (*.json)");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filename;
        return;
    }
    QByteArray file_data = file.readAll();
    file.close();
    QJsonParseError parse_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(file_data);
    if (parse_error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parse_error.errorString();
        return;
    }
    if (!json_doc.isObject()) {
        qWarning() << "Invalid JSON format: expected an object at the root.";
        return;
    }
    QJsonObject root_obj = json_doc.object();
    if (!root_obj.contains("flight_plan") ||
        !root_obj["flight_plan"].isArray()) {
        qWarning() << "Invalid JSON format: expected 'flight_plan' array.";
        return;
    }
    QJsonArray flight_plan_array = root_obj["flight_plan"].toArray();
    std::list<Waypoint> waypoints;
    for (int i = 0; i < flight_plan_array.size(); ++i) {
        QJsonValue val = flight_plan_array.at(i);
        if (!val.isObject()) {
            qWarning() << "Invalid JSON format: Skipping" << val;
            continue;
        }
        QJsonObject wp_obj = val.toObject();
        Waypoint wp;
        bool valid = true;
        if (wp_obj.contains("name") && wp_obj["name"].isString()) {
            wp.name = wp_obj["name"].toString("UNKNOWN");
        } else {
            qWarning() << "Invalid JSON format: Waypoint missing 'name' field, "
                          "skipping.";
            valid = false;
        }

        if (wp_obj.contains("lat") && wp_obj["lat"].isDouble()) {
            wp.lat = wp_obj["lat"].toDouble(0.0);
        } else {
            qWarning() << "Invalid JSON format: Waypoint missing 'lat' field, "
                          "skipping.";
            valid = false;
        }
        if (wp_obj.contains("lon") && wp_obj["lon"].isDouble()) {
            wp.lon = wp_obj["lon"].toDouble(0.0);
        } else {
            qWarning() << "Invalid JSON format: Waypoint missing 'lon' field, "
                          "skipping.";
            valid = false;
        }
        if (valid) {
            waypoints.emplace_back(wp);
        }
    }
    if (this->m_nav_map_panel && !waypoints.empty()) {
        m_nav_map_panel->clearWaypoints();
        qDebug() << "Loaded flight plan with: ";
        std::for_each(
            waypoints.begin(), waypoints.end(), [this](const Waypoint& wp) {
                qDebug() << "Waypoint:" << wp.name << wp.lat << wp.lon;
                m_nav_map_panel->addWaypoint(wp);
            });
    }
}
void MFD::FPLNForcedClear() {
    m_nav_map_panel->clearWaypoints();
}
void MFD::ZoomLevelChanged() {
    constexpr double zoom_levels[] = {5000.0, 3000.0, 1500.0, 750.0, 375.0};
    constexpr uint8_t num_levels = 5;
    static uint8_t zoom_index = 0;
    m_nav_map_panel->setZoom(zoom_levels[zoom_index++]);
    if (zoom_index >= 5) {
        zoom_index = 0;
    }
}
