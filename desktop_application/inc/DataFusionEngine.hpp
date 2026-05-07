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
 * @author Maksymilian Ksiazka
 * @date 2026-05-07
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
    /**
     * @brief Handles incoming sensor data from the SensorLink and updates the
     * Kalman filter state accordingly.
     *
     * @param[in] data -- The received sensor data from the SensorLink, \link
     * SensorData \endlink struct.
     */
    void handleSensorData(const SensorData& data);
  signals:
    /**
     * @brief Emitted when the orientation estimate is updated after processing
     * new sensor data.
     *
     * @param[in] orientation -- The updated orientation estimate as a
     * quaternion.
     */
    void orientationUpdated(const Eigen::Quaterniond& orientation);

  public:
    /**
     * @brief Constructs a new DataFusionEngine object and initializes the
     * matrices.
     *
     */
    DataFusionEngine();
    /**
     * @brief Performs the prediction step of the Kalman filter using gyroscope
     * data.
     *
     * @param[in] gyro -- The gyroscope measurements as a 3D vector.
     * @param[in] dt -- The time step in seconds since the last update.
     */
    void predict(const Eigen::Vector3d& gyro, double dt);
    /**
     * @brief Performs the update step of the Kalman filter using accelerometer
     * data to correct the orientation estimate.
     *
     * @param[in] accel -- The accelerometer measurements as a 3D vector,
     * typically representing the gravity vector in the sensor frame.
     */
    void update(const Eigen::Vector3d& accel);
    /**
     * @brief Converts the current orientation estimate from quaternion form to
     * roll and pitch angles.
     *
     * @retval A pair of doubles representing the roll and pitch angles in
     * radians.
     */
    RollPitch getRollPitch() const;
    /**
     * @brief Connects the DataFusionEngine instance to a SensorLink instance to
     * receive sensor data updates.
     *
     * @param[in] sensor_link -- Pointer to \link SensorLink \endlink instance
     */
    void connectToSensorLink(SensorLink* sensor_link);
};
#endif // DATAFUSIONENGINE_H_
