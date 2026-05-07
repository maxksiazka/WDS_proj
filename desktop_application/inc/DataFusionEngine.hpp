#ifndef DATAFUSIONENGINE_H_
#define DATAFUSIONENGINE_H_
#include "SensorLink.hpp"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <QObject>
/**
 * @file DataFusionEngine.hpp
 * @brief Defines the DataFusionEngine class for fusing sensor data using a
 * Kalman filter.
 *
 * This class implements an Extended Kalman filter to fuse data from multiple
 * sensors, such as IMU, GPS, and pressure sensors.
 *
 */

typedef std::pair<double, double> RollPitch;

class DataFusionEngine : public QObject {
    Q_OBJECT
  private:
    Eigen::Quaterniond orientation_;
    Eigen::Matrix4d covariance_;
    Eigen::Matrix4d Q; // Process noise covariance
    Eigen::Matrix3d R; // Measurement noise covariance
    double dt_;
  private slots:
    void handleSensorData(const SensorData& data);
  signals:
    void orientationUpdated(const Eigen::Quaterniond& orientation);

  public:
    DataFusionEngine();
    void predict(const Eigen::Vector3d& gyro, double dt);
    void update(const Eigen::Vector3d& accel);
    RollPitch getRollPitch() const;
    void connectToSensorLink(SensorLink* sensor_link);
};
#endif // DATAFUSIONENGINE_H_
