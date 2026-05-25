#include "DataFusionEngine.hpp"
#include <Eigen/Core>
#include <QDebug>

DataFusionEngine::DataFusionEngine() {
    m_covariance = Eigen::Matrix4d::Identity() * 0.1;
    m_orientation = Eigen::Quaterniond::Identity();
    m_Q = Eigen::Matrix4d::Identity() * 0.0001;
    m_R = Eigen::Matrix3d::Identity() * 0.01;
    // bias will drift more than orientation, so we set higher process noise for
    // it
    m_heading_Q << 0.001, 0, 0, 0.003;
}
void DataFusionEngine::connectToSensorLink(SensorLink* sensor_link) {
    connect(sensor_link, SIGNAL(data_received(const SensorData&)), this,
            SLOT(handleSensorData(const SensorData&)));
}
void DataFusionEngine::handleSensorData(const SensorData& data) {
    Eigen::Vector3d gyro(data.gyro[0], data.gyro[1], data.gyro[2]);
    Eigen::Vector3d accel(data.accel[0], data.accel[1], data.accel[2]);
    // qDebug() << "Gyro:" << data.gyro[0] << data.gyro[1] << data.gyro[2];
    // qDebug() << "Accel:" << data.accel[0] << data.accel[1] << data.accel[2];

    predictOrientation(gyro, m_dt);
    updateOrientation(accel);
    emit orientationUpdated(m_orientation);

    // remove gravity component from accel to get true forward acceleration
    Eigen::Vector3d gravity =
        m_orientation.inverse() * Eigen::Vector3d(0, 0, -9.81);
    Eigen::Vector3d true_accel = accel - gravity;
    double forward_accel = true_accel.x();
    updateAirspeed(data.airspeed, forward_accel);
    emit airspeedUpdated(m_airspeed_estimate);
    updateAltitude(data.pressure);
    emit altitudeUpdated(m_altitude_estimate);
    updateHeading(data.mag[1],data.mag[0], data.gyro[2]);
    emit headingUpdated(m_heading_estimate(0));
}
void DataFusionEngine::predictOrientation(const Eigen::Vector3d& gyro, double dt) {
    double gx = gyro.x();
    double gy = gyro.y();
    double gz = gyro.z();

    Eigen::Matrix4d Omega;
    Omega << 0, -gx, -gy, -gz, gx, 0, gz, -gy, gy, -gz, 0, gx, gz, gy, -gx, 0;
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity() + (0.5 * Omega * dt);

    m_orientation = F * m_orientation.coeffs();
    m_orientation.normalize();

    m_covariance =
        (F * m_covariance * F.transpose()) + Eigen::Matrix4d::Identity() * m_Q;
}
void DataFusionEngine::updateOrientation(const Eigen::Vector3d& accel) {
    double q0 = m_orientation.w();
    double q1 = m_orientation.x();
    double q2 = m_orientation.y();
    double q3 = m_orientation.z();
    Eigen::Vector3d h;
    h.x() = 2 * (q1 * q3 - q0 * q2);
    h.y() = 2 * (q0 * q1 + q2 * q3);
    h.z() = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    Eigen::Matrix<double, 3, 4> H;
    H << -2.0 * q2, 2.0 * q3, -2.0 * q0, 2.0 * q1, 2.0 * q1, 2.0 * q0, 2.0 * q3,
        2.0 * q2, 2.0 * q0, -2.0 * q1, -2.0 * q2, 2.0 * q3;

    Eigen::Vector3d z = accel.normalized();
    Eigen::Vector3d y = z - h;

    Eigen::Matrix3d S = H * m_covariance * H.transpose() + m_R;
    Eigen::Matrix<double, 4, 3> K = m_covariance * H.transpose() * S.inverse();

    Eigen::Vector4d delta = K * y;
    m_orientation.coeffs() += delta;

    m_orientation.normalize();
    m_covariance = (Eigen::Matrix4d::Identity() - K * H) * m_covariance;

    m_covariance = (m_covariance + m_covariance.transpose()) * 0.5;
}
void DataFusionEngine::updateAirspeed(double raw_airspeed, double forward_accel) {
    m_airspeed_estimate += forward_accel * m_dt;
    m_airspeed_variance += m_airspeed_Q;

    double y = raw_airspeed - m_airspeed_estimate;
    double S = m_airspeed_variance + m_airspeed_R;
    double K = m_airspeed_variance / S;

    m_airspeed_estimate += K * y;
    m_airspeed_variance *= (1 - K);
    qDebug() << "Airspeed update: " << m_airspeed_estimate;
}
void DataFusionEngine::updateAltitude(double raw_pressure) {
    double raw_alt_meters =
        44330.0 * (1.0 - std::pow(raw_pressure / m_qnh_pa, 0.1902949));
    double raw_alt_feet = raw_alt_meters * 3.28084;

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
    double measured_heading = std::atan2(mag_y, mag_x) * (180.0 / M_PI);
    if (measured_heading < 0)
        measured_heading += 360.0;
    double gyro_z_deg = gyro_z * (180.0 / M_PI);
    Eigen::Matrix2d F;
    F << 1.0, -m_dt, 0.0, 1.0;
    m_heading_estimate(0) += (gyro_z_deg - m_heading_estimate(1)) * m_dt;

    m_heading_covariance =
        (F * m_heading_covariance * F.transpose()) + m_heading_Q;

    Eigen::RowVector2d H(1.0, 0.0);

    double y = measured_heading - m_heading_estimate(0);
    while (y > 180.0)
        y -= 360.0;
    while (y < -180.0)
        y += 360.0;
    double S = (H * m_heading_covariance * H.transpose())(0, 0) + m_heading_R;

    Eigen::Vector2d K = m_heading_covariance * H.transpose() / S;

    m_heading_estimate += K * y;

    m_heading_covariance =
        (Eigen::Matrix2d::Identity() - K * H) * m_heading_covariance;

    while (m_heading_estimate(0) < 0)
        m_heading_estimate(0) += 360.0;
    while (m_heading_estimate(0) >= 360.0)
        m_heading_estimate(0) -= 360.0;
}
void DataFusionEngine::handleQNHKnobChange(double qnh_value) {
    m_qnh_pa = qnh_value;
}
