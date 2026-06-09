#include "DataFusionEngine.hpp"
#include <Eigen/Core>
#include <QDebug>

DataFusionEngine::DataFusionEngine() {
    m_dt = 0.02; // we expect 50Hz, but will update this based on actual
                 // timestamps in the future
    m_covariance = Eigen::Matrix4d::Identity() * 0.1;
    m_orientation = Eigen::Quaterniond::Identity();
    m_Q = Eigen::Matrix4d::Identity() * 0.0001;
    m_R = Eigen::Matrix3d::Identity() * 0.5;

    m_heading_covariance.setZero();
    m_heading_covariance(0, 0) = 10.0;
    m_heading_covariance(1, 1) = 0.5;
    m_heading_covariance(2, 2) = 1000.0;
    m_heading_covariance(3, 3) = 1000.0;

    m_heading_Q.setZero();
    m_heading_Q(0, 0) = 0.001;
    m_heading_Q(1, 1) = 0.0001;
    m_heading_Q(2, 2) = 0.01;
    m_heading_Q(3, 3) = 0.01;
}
void DataFusionEngine::connectToSensorLink(SensorLink* sensor_link) {
    connect(sensor_link, SIGNAL(data_received(const SensorData&)), this,
            SLOT(handleSensorData(const SensorData&)));
}
void DataFusionEngine::handleSensorData(const SensorData& data) {
    static bool first_update = true;
    if (first_update) {
        m_last_timestamp_us = data.timestamp_us;
        first_update = false;
    } else {
        if (data.timestamp_us <
            m_last_timestamp_us) { // hardware reset or timestamp overflow,
                                   // reset filter
            m_last_timestamp_us = data.timestamp_us;
            m_dt = 0.02;
        } else {
            m_dt = (data.timestamp_us - m_last_timestamp_us) / 1e6;
            m_last_timestamp_us = data.timestamp_us;
        }
    }
    // EKF guard
    if (m_dt < 0.005 || m_dt > 0.5) {
        m_dt = 0.02;
    }
    const Eigen::Vector3d gyro(data.gyro[0], data.gyro[1], data.gyro[2]);
    const Eigen::Vector3d accel(data.accel[0], data.accel[1], data.accel[2]);
    // qDebug() << "Gyro:" << data.gyro[0] << data.gyro[1] << data.gyro[2];
    // qDebug() << "Accel:" << data.accel[0] << data.accel[1] << data.accel[2];

    predictOrientation(gyro, m_dt);
    updateOrientation(accel);
    emit orientationUpdated(m_orientation);

    // remove gravity component from accel to get true forward acceleration
    Eigen::Vector3d gravity =
        m_orientation.inverse() * Eigen::Vector3d(0, 0, -9.81);
    Eigen::Vector3d true_accel = accel - gravity;
    const double forward_accel = true_accel.x();
    updateAirspeed(data.airspeed, forward_accel);
    emit airspeedUpdated(m_airspeed_estimate);
    updateAltitude(data.pressure);
    emit altitudeUpdated(m_altitude_estimate);
    updateHeading(data.mag[1], data.mag[0], data.gyro[2]);
    double heading_deg = m_heading_estimate(0) * (180.0 / M_PI);
    if (heading_deg < 0) {
        heading_deg += 360.0;
    }
    emit headingUpdated(heading_deg);
    emit GPSHeadingUpdated(data.gps_lat, data.gps_lon, heading_deg);
    emit temperatureGPSUpdated(data.temperature, data.gps_sats, data.gps_fix);
}
void DataFusionEngine::predictOrientation(const Eigen::Vector3d& gyro,
                                          const double dt) {
    const double gx = gyro.x();
    const double gy = gyro.y();
    const double gz = gyro.z();

    Eigen::Matrix4d Omega;
    Omega << 0, -gx, -gy, -gz, gx, 0, gz, -gy, gy, -gz, 0, gx, gz, gy, -gx, 0;
    const Eigen::Matrix4d F = Eigen::Matrix4d::Identity() + (0.5 * Omega * dt);

    const Eigen::Vector4d q_vec(m_orientation.w(), m_orientation.x(),
                                m_orientation.y(), m_orientation.z());
    const Eigen::Vector4d predicted_q_vec = F * q_vec;
    m_orientation.w() = predicted_q_vec(0);
    m_orientation.x() = predicted_q_vec(1);
    m_orientation.y() = predicted_q_vec(2);
    m_orientation.z() = predicted_q_vec(3);
    m_orientation.normalize();

    m_covariance =
        (F * m_covariance * F.transpose()) + Eigen::Matrix4d::Identity() * m_Q;
}
void DataFusionEngine::updateOrientation(const Eigen::Vector3d& accel) {
    const double q0 = m_orientation.w();
    const double q1 = m_orientation.x();
    const double q2 = m_orientation.y();
    const double q3 = m_orientation.z();
    Eigen::Vector3d h;
    h.x() = 2 * (q1 * q3 - q0 * q2);
    h.y() = 2 * (q0 * q1 + q2 * q3);
    h.z() = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    Eigen::Matrix<double, 3, 4> H;
    H << -2.0 * q2, 2.0 * q3, -2.0 * q0, 2.0 * q1, 2.0 * q1, 2.0 * q0, 2.0 * q3,
        2.0 * q2, 2.0 * q0, -2.0 * q1, -2.0 * q2, 2.0 * q3;

    const Eigen::Vector3d z = accel.normalized();
    const Eigen::Vector3d y = z - h;

    const Eigen::Matrix3d S = H * m_covariance * H.transpose() + m_R;
    const Eigen::Matrix<double, 4, 3> K =
        m_covariance * H.transpose() * S.inverse();

    const Eigen::Vector4d delta = K * y;
    m_orientation.w() += delta(0);
    m_orientation.x() += delta(1);
    m_orientation.y() += delta(2);
    m_orientation.z() += delta(3);
    m_orientation.normalize();

    m_covariance = (Eigen::Matrix4d::Identity() - K * H) * m_covariance;

    m_covariance = (m_covariance + m_covariance.transpose()) * 0.5;
}
void DataFusionEngine::updateAirspeed(double raw_airspeed,
                                      double forward_accel) {
    m_airspeed_estimate += forward_accel * m_dt;
    m_airspeed_variance += m_airspeed_Q;

    const double y = raw_airspeed - m_airspeed_estimate;
    const double S = m_airspeed_variance + m_airspeed_R;
    const double K = m_airspeed_variance / S;

    m_airspeed_estimate += K * y;
    m_airspeed_variance *= (1 - K);
    qDebug() << "Airspeed update: " << m_airspeed_estimate;
}
void DataFusionEngine::updateAltitude(double raw_pressure) {
    const double raw_alt_meters =
        44330.0 * (1.0 - std::pow(raw_pressure / m_qnh_pa, 0.1902949));
    const double raw_alt_feet = raw_alt_meters * 3.28084;

    static bool first_update = true;
    if (first_update) {
        m_altitude_estimate = raw_alt_feet;
        first_update = false;
    } else {
        m_altitude_estimate = (m_alpha_alt * raw_alt_feet) +
                              ((1 - m_alpha_alt) * m_altitude_estimate);
    }
}
void DataFusionEngine::updateHeading(float mag_y, float mag_x, double gyro_z) {
    
    Eigen::Vector2d z_measured(static_cast<double>(mag_x), static_cast<double>(mag_y));
    if (z_measured.norm() > 0.01) {
        z_measured.normalize();
    }
    static bool first_update = true;
    if (first_update) {
        m_heading_estimate(0) = std::atan2(mag_y, mag_x);
        m_heading_estimate(1) = 0.0;
        m_heading_estimate(2) = 0.0;
        m_heading_estimate(3) = 0.0;
        first_update = false;
        return;
    }
    const double gyro_z_rad = -gyro_z;
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity();
    F(0, 1) = -m_dt;

    double unbiased_gyro_z = gyro_z_rad - m_heading_estimate(1);
    m_heading_estimate(0) += unbiased_gyro_z * m_dt;
    m_heading_covariance =
        (F * m_heading_covariance * F.transpose()) + m_heading_Q;
    const double psi_rad = m_heading_estimate(0);
    Eigen::Vector2d z_predicted;
    z_predicted.x() = std::cos(psi_rad) + m_heading_estimate(2);
    z_predicted.y() = std::sin(psi_rad) + m_heading_estimate(3);
    Eigen::Vector2d y = z_measured - z_predicted;

    Eigen::Matrix<double, 2, 4> H = Eigen::Matrix<double, 2, 4>::Zero();
    H(0, 0) = -std::sin(psi_rad);
    H(1, 0) = std::cos(psi_rad);
    H(0, 2) = 1.0;
    H(1, 3) = 1.0;
    Eigen::Matrix2d R = Eigen::Matrix2d::Identity() * m_heading_R;
    Eigen::Matrix2d S = H * m_heading_covariance * H.transpose() + R;
    Eigen::Matrix<double, 4, 2> K =
        m_heading_covariance * H.transpose() * S.inverse();

    m_heading_estimate += K * y;

    Eigen::Matrix4d I = Eigen::Matrix4d::Identity();
    m_heading_covariance = (I - K * H) * m_heading_covariance;
    m_heading_covariance =
        (m_heading_covariance + m_heading_covariance.transpose()) * 0.5;
    //cleanup stage
    while (m_heading_estimate(0) >= M_PI) {
        m_heading_estimate(0) -= 2 * M_PI;
    }
    while (m_heading_estimate(0) < -M_PI) {
        m_heading_estimate(0) += 2 * M_PI;
    }
}
void DataFusionEngine::handleQNHKnobChange(double qnh_value) {
    m_qnh_pa = qnh_value;
}
