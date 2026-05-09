#include "DataFusionEngine.hpp"
#include <Eigen/Core>

DataFusionEngine::DataFusionEngine() {
    covariance_ = Eigen::Matrix4d::Identity() * 0.1;
    orientation_ = Eigen::Quaterniond::Identity();
    Q = Eigen::Matrix4d::Identity() * 0.0001;
    R = Eigen::Matrix3d::Identity() * 0.01;
}
void DataFusionEngine::connectToSensorLink(SensorLink* sensor_link) {
    connect(sensor_link,SIGNAL(data_received()), this,SLOT(handleSensorData()) );
}
// TODO: Verify if 60Hz is even possible to handle, consider using a separate thread for data processing if needed
void DataFusionEngine::handleSensorData(const SensorData& data) {
    Eigen::Vector3d gyro(data.gyro[0], data.gyro[1], data.gyro[2]);
    Eigen::Vector3d accel(data.accel[0], data.accel[1], data.accel[2]);
    predict(gyro, dt_);
    update(accel);
    emit orientationUpdated(orientation_);
}
void DataFusionEngine::predict(const Eigen::Vector3d& gyro, double dt) {
    double gx = gyro.x();
    double gy = gyro.y();
    double gz = gyro.z();

    Eigen::Matrix4d Omega;
    Omega << 0, -gx, -gy, -gz, gx, 0, gz, -gy, gy, -gz, 0, gx, gz, gy, -gx, 0;
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity() + (0.5 * Omega * dt);

    orientation_ = F * orientation_.coeffs();
    orientation_.normalize();

    covariance_ =
        (F * covariance_ * F.transpose()) + Eigen::Matrix4d::Identity() * Q;
}
void DataFusionEngine::update(const Eigen::Vector3d& accel) {
    double q0 = orientation_.w();
    double q1 = orientation_.x();
    double q2 = orientation_.y();
    double q3 = orientation_.z();
    Eigen::Vector3d h;
    h.x() = 2 * (q1 * q3 - q0 * q2);
    h.y() = 2 * (q0 * q1 + q2 * q3);
    h.z() = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    Eigen::Matrix<double, 3, 4> H;
    H << -2.0 * q2, 2.0 * q3, -2.0 * q0, 2.0 * q1, 2.0 * q1, 2.0 * q0, 2.0 * q3,
        2.0 * q2, 2.0 * q0, -2.0 * q1, -2.0 * q2, 2.0 * q3;

    Eigen::Vector3d z = accel.normalized();
    Eigen::Vector3d y = z - h;

    Eigen::Matrix3d S = H * covariance_ * H.transpose() + R;
    Eigen::Matrix<double, 4, 3> K = covariance_ * H.transpose() * S.inverse();

    Eigen::Vector4d delta = K * y;
    orientation_.coeffs() += delta;

    orientation_.normalize();
    covariance_ = (Eigen::Matrix4d::Identity() - K * H) * covariance_;

    covariance_ = (covariance_ + covariance_.transpose()) * 0.5;
}
