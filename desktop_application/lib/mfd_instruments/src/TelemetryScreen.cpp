#include "TelemetryScreen.hpp"
#include <QGroupBox>
#include <QTimer>
#include <QVBoxLayout>
#include <cmath>

TelemetryScreen::TelemetryScreen(QWidget* parent) : QFrame(parent) {
    setStyleSheet(
        "QFrame { background-color: #1a1a1a;}"
        "QGroupBox { color: #8c8c8c; font-weight: bold; font-size: 11px; "
        "border: 1px solid #222222; margin-top: 12px; padding-top: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: "
        "top left;}");

    QVBoxLayout* main_layout = new QVBoxLayout(this);

    QLabel* header = new QLabel("SYS TELEMETRY / DIAGNOSTICS", this);
    header->setStyleSheet(
        "QLabel { color: #8c8c8c; font-weight: bold; font-size: 11px; border: "
        "none; background: transparent; }");
    header->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(header);

    QGroupBox* conn_data_box = new QGroupBox("LINK INTERFACE", this);
    QGridLayout* conn_data_grid = new QGridLayout(conn_data_box);
    addTelemetryRow(conn_data_grid, 0, "LINK STATE:", m_lbl_conn_state);
    main_layout->addWidget(conn_data_box);

    QGroupBox* system_core_box = new QGroupBox("SYSTEM CORE", this);
    QGridLayout* system_core_grid = new QGridLayout(system_core_box);
    addTelemetryRow(system_core_grid, 0, "MCU UPTIME:", m_lbl_uptime);
    addTelemetryRow(system_core_grid, 1, "BARO PRESS:", m_lbl_pressure);
    main_layout->addWidget(system_core_box);

    QGroupBox* gps_box = new QGroupBox("GNSS DIAGNOSTICS", this);
    QGridLayout* gps_grid = new QGridLayout(gps_box);
    addTelemetryRow(gps_grid, 0, "FIX STATUS:", m_lbl_gps_fix);
    addTelemetryRow(gps_grid, 1, "SATS IN VIEW:", m_lbl_gps_sats);
    addTelemetryRow(gps_grid, 2, "POSITION:", m_lbl_coords);
    main_layout->addWidget(gps_box);

    QGroupBox* perf_box = new QGroupBox("SUPP FLIGHT DATA", this);
    QGridLayout* perf_grid = new QGridLayout(perf_box);
    addTelemetryRow(perf_grid, 0, "GROUND SPEED:", m_lbl_ground_speed);
    addTelemetryRow(perf_grid, 1, "WIND COMPONENT:", m_lbl_wind_component);
    addTelemetryRow(perf_grid, 2, "G-LOAD FACTOR:", m_lbl_gforce);
    main_layout->addWidget(perf_box);

    m_watchdog_timer = new QTimer(this);
    m_watchdog_timer->setInterval(3000);
    connect(m_watchdog_timer, &QTimer::timeout, this,
            &TelemetryScreen::handleWatchdogTimeout);

    setDisconnectedUI();
}

void TelemetryScreen::updateTelemetryData(const TelemetryData& data) {
    qDebug() << "Updated telemetry data received.";
    m_watchdog_timer->start();
    m_lbl_conn_state->setText("<span style='color:#00ff00;'>CONNECTED</span>");
    updateUptime(data.timestamp_us);
    updateBaroPressure(data.pressure_hpa);
    updateGNSSDiagnostics(data.gps_fix_type, data.gps_sats_count, data.gps_lat,
                          data.gps_lon);
    updateKinematicsAndWind(data.airspeed_kt, data.ground_speed_kt);
    updateAccelerationVectors(data.accel_x, data.accel_y, data.accel_z);
    update();
}

void TelemetryScreen::updateUptime(uint64_t timestamp_us) {
    const uint64_t total_sec = timestamp_us / 1000000;
    const std::div_t time_parts = std::div(total_sec, 3600);
    const std::div_t rem_minutes = std::div(time_parts.rem, 60);
    const int32_t hours = time_parts.quot;
    const int32_t minutes = rem_minutes.quot;
    const int32_t seconds = rem_minutes.rem;
    m_lbl_uptime->setText(QString("%1:%2:%3")
                              .arg(hours, 2, 10, QChar('0'))
                              .arg(minutes, 2, 10, QChar('0'))
                              .arg(seconds, 2, 10, QChar('0')));
}

void TelemetryScreen::updateBaroPressure(float pressure_hpa) {
    m_lbl_pressure->setText(QString("%1 hPa").arg(pressure_hpa, 0, 'f', 2));
}

void TelemetryScreen::updateGNSSDiagnostics(uint8_t fix_type,
                                            uint8_t sats_count, double lat,
                                            double lon) {
    QString fix_str = "UNKNOWN";
    if (fix_type == 0)
        fix_str = "<span style='color:#ff0000;'>NO FIX</span>";//red
    else if (fix_type == 1)
        fix_str = "<span style='color:#ffff00;'>2D FIX</span>";//yellow
    else if (fix_type == 2)
        fix_str = "<span style='color:#00ff00;'>3D FIX</span>";//green
    m_lbl_gps_fix->setText(fix_str);

    m_lbl_gps_sats->setText(
        QString("%1 SATS").arg(static_cast<int>(sats_count)));
    m_lbl_coords->setText(
        QString("%1N, %2E").arg(lat, 0, 'f', 5).arg(lon, 0, 'f', 5));
}

void TelemetryScreen::updateKinematicsAndWind(float airspeed_kt,
                                              float ground_speed_kt) {
    m_lbl_ground_speed->setText(
        QString("%1 KT").arg(ground_speed_kt, 0, 'f', 1));

    const float wind_delta = ground_speed_kt - airspeed_kt;
    if (std::abs(wind_delta) < 0.5f) {
        m_lbl_wind_component->setText("CALM / NEUTRAL");
    } else if (wind_delta > 0.0f) {
        m_lbl_wind_component->setText(
            QString("<span style='color:#00ff00;'>TAILWIND: %1 KT</span>")
                .arg(wind_delta, 0, 'f', 1));
    } else {
        m_lbl_wind_component->setText(
            QString("<span style='color:#ffaa00;'>HEADWIND: %1 KT</span>")
                .arg(std::abs(wind_delta), 0, 'f', 1));
    }
}

void TelemetryScreen::updateAccelerationVectors(float ax, float ay, float az) {
    const double structural_magnitude = std::sqrt(ax * ax + ay * ay + az * az);
    const double g_load = structural_magnitude / 9.80665;

    if (g_load > 3.8 || g_load < -1.5) {
        m_lbl_gforce->setText(
            QString("<span style='color:#ff0000;'>%1 G [OVERLOAD]</span>")
                .arg(g_load, 0, 'f', 2));
    } else {
        m_lbl_gforce->setText(QString("%1 G").arg(g_load, 0, 'f', 2));
    }
}

void TelemetryScreen::addTelemetryRow(QGridLayout* grid, int row,
                                      const QString& label_text,
                                      QLabel*& value_label) {
    QLabel* lbl_title = new QLabel(label_text, this);
    lbl_title->setStyleSheet(
        "QLabel { color: #8c8c8c; font-size: 11px; font-family: 'Courier New'; "
        "border: none; background: transparent; }");

    value_label = new QLabel("---", this);
    value_label->setStyleSheet(
        "QLabel { color: #8c8c8c; font-weight: bold; font-size: 11px; "
        "font-family: 'Courier New'; border: none; background: transparent; }");
    value_label->setAlignment(Qt::AlignRight);

    grid->addWidget(lbl_title, row, 0);
    grid->addWidget(value_label, row, 1);
}
void TelemetryScreen::setDisconnectedUI() {
    m_lbl_conn_state->setText(
        "<span style='color:#ff0000;'>DISCONNECTED</span>");
    m_lbl_uptime->setText("--:--:--");
    m_lbl_pressure->setText("--- hPa");
    m_lbl_gps_fix->setText("<span style='color:#ff0000;'> N/A </span>");
    m_lbl_gps_sats->setText("- SATS");
    m_lbl_coords->setText("---, ---");
    m_lbl_ground_speed->setText("--- KT");
    m_lbl_wind_component->setText("---");
    m_lbl_gforce->setText("--- G");
}
void TelemetryScreen::handleWatchdogTimeout() {
    m_watchdog_timer->stop();
    setDisconnectedUI();
    m_lbl_conn_state->setText(
        "<span style='color:#ffaa00;'>LINK TIMEOUT</span>");
}
