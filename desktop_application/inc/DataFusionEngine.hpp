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
    Eigen::Quaterniond m_orientation;
    Eigen::Matrix4d m_covariance;
    Eigen::Matrix4d m_Q; // Process noise covariance
    Eigen::Matrix3d m_R; // Measurement noise covariance
    double m_dt;
    double m_airspeed_estimate = 0.0;
    double m_airspeed_variance = 1.0;

    const double m_airspeed_Q = 0.01;
    const double m_airspeed_R = 0.5;
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
     * @brief Updates the airspeed estimate using a simple Kalman filter based on the airspeed measurement from the sensor data.
     *
     * @param[in] raw_airspeed -- The raw airspeed measurement from the sensor data.
     * @param[in] forward_accel -- The true forward acceleration -- derived from the orientation and accelerometer data -- used to predict changes in airspeed.
     * @param[in] dt -- The time step in seconds since the last update.
     *
     * @warning This method is slightly vulnerable to centripetal forces during turns, which can cause the 
     * forward accel to be overestimated. In practice, just dont use such simple math for fighter jets.
     */
    void updateAirspeed(double raw_airspeed, double forward_accel, double dt);
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


    /**
     * @brief Emitted when the airspeed estimate is updated after processing new sensor data.
     *
     * @param[in] airspeed -- The updated airspeed estimate in knots.
     */
    void airspeedUpdated(double airspeed);
  public:
    /**
     * @brief Constructs a new DataFusionEngine object and initializes the
     * matrices.
     *
     */
    DataFusionEngine();
    /**
     * @brief Connects the DataFusionEngine instance to a SensorLink instance to
     * receive sensor data updates.
     *
     * @param[in] sensor_link -- Pointer to \link SensorLink \endlink instance
     */
    void connectToSensorLink(SensorLink* sensor_link);
};
#endif // DATAFUSIONENGINE_H_
