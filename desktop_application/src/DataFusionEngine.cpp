#include "DataFusionEngine.hpp"
#include <Eigen/Core>
#include <QDebug>

DataFusionEngine::DataFusionEngine() {
    m_covariance = Eigen::Matrix4d::Identity() * 0.1;
    m_orientation = Eigen::Quaterniond::Identity();
    m_Q = Eigen::Matrix4d::Identity() * 0.0001;
    m_R = Eigen::Matrix3d::Identity() * 0.01;
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

    predict(gyro, m_dt);
    update(accel);
    emit orientationUpdated(m_orientation);

    // remove gravity component from accel to get true forward acceleration
    Eigen::Vector3d gravity =
        m_orientation.inverse() * Eigen::Vector3d(0, 0, -9.81);
    Eigen::Vector3d true_accel = accel - gravity;
    double forward_accel = true_accel.x();
    updateAirspeed(data.airspeed, forward_accel, m_dt);
    emit airspeedUpdated(m_airspeed_estimate);
}
void DataFusionEngine::predict(const Eigen::Vector3d& gyro, double dt) {
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
void DataFusionEngine::update(const Eigen::Vector3d& accel) {
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
void DataFusionEngine::updateAirspeed(double raw_airspeed, double forward_accel,
                                      double dt) {
    m_airspeed_estimate += forward_accel * dt;
    m_airspeed_variance += m_airspeed_Q;

    double y = raw_airspeed - m_airspeed_estimate;
    double S = m_airspeed_variance + m_airspeed_R;
    double K = m_airspeed_variance / S;

    m_airspeed_estimate += K * y;
    m_airspeed_variance *= (1 - K);
    qDebug() << "Airspeed update: " << m_airspeed_estimate;
}
