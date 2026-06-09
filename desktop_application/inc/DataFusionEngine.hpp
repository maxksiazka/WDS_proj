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
 * @note Thanks to kalmanfilter.net for the mathematical explanations.
 * @author Maksymilian Ksiazka
 * @date 2026-05-07
 */

/**
 * @brief DataFusionEngine class responsible for fusing and filtering sensor
 * data.
 *
 * The DataFusionEngine class is the main data provider of this simulation. It
 * receives raw sensor data from the SensorLink, processes it using:
 *  - An Extended Kalman filter for orientation estimation.
 *  - A 1D Kalman filter for airspeed estimation.
 *  - A complementary filter for altitude estimation.
 *  - A 2D Kalman filter for heading estimation.
 *  and emits signals with the fused and filtered data to update the UI
 * components.
 * */
class DataFusionEngine : public QObject {
    Q_OBJECT
  private:
    /**
     * @brief -- The current orientation estimate of the aircraft represented as
     * a quaternion.
     */
    Eigen::Quaterniond m_orientation;
    /**
     * @brief -- State covariance matrix representing the uncertainty in the
     * orientation.
     */
    Eigen::Matrix4d m_covariance;
    /**
     * @brief -- Process noise covariance matrix for the orientation state.
     */
    Eigen::Matrix4d m_Q;
    /**
     * @brief -- Measurement noise covariance matrix for the accelerometer
     * measurements.
     */
    Eigen::Matrix3d m_R;
    /**
     * @brief -- The time step (in seconds) since the last update.
     */
    double m_dt;
    /**
     * @brief -- The current estimate of the airspeed in knots.
     */
    double m_airspeed_estimate = 0.0;
    /**
     * @brief -- Predicted state variance for the airspeed estimate.
     */
    double m_airspeed_variance = 1.0;

    /**
     * @brief -- Process noise variance for the airspeed estimate.
     */
    static constexpr double m_airspeed_Q = 0.01;
    /**
     * @brief -- Measurement noise variance for the airspeed estimate.
     */
    static constexpr double m_airspeed_R = 0.7;
    /**
     * @brief -- The current estimate of the altitude in feet, calculated from
     * the pressure sensor data and QNH setting.
     */
    double m_altitude_estimate = 0.0;
    /**
     * @brief -- The current QNH setting in Pa, used for altitude calculations
     * and airspeed corrections.
     */
    double m_qnh_pa = 101325.0;
    /**
     * @brief -- The timestamp of the last received sensor data in microseconds.
     */
    uint64_t m_last_timestamp_us = 0;

    /**
     * @brief -- Smoothing factor for altitude estimation.
     */
    static constexpr double m_alpha_alt = 0.1;

    /**
     * @brief -- The current estimate of the heading angle in
     * radians.
     *
     * Represented as 4D vector, [heading, gyro_drift, mag_bias_X, mag_bias_Y].
     */
    Eigen::Vector4d m_heading_estimate = Eigen::Vector4d::Zero();
    /**
     * @brief -- State covariance matrix representing the
     * uncertainty in the heading estimate.
     */
    Eigen::Matrix4d m_heading_covariance = Eigen::Matrix4d::Identity() * 0.1;
    /**
     * @brief -- Process noise covariance matrix for the heading state,
     * representing the uncertainty in the heading dynamics and bias drift.
     */
    Eigen::Matrix4d m_heading_Q;
    /**
     * @brief -- Measurement noise covariance for the heading measurement,
     * representing the uncertainty in the magnetometer measurements.
     */
    static constexpr double m_heading_R = 10.0;
    /**
     * @brief Performs the prediction step of the Kalman filter using gyroscope
     * data.
     *
     * @param[in] gyro -- The gyroscope measurements as a 3D vector.
     * @param[in] dt -- The time step in seconds since the last update.
     */
    void predictOrientation(const Eigen::Vector3d& gyro, double dt);
    /**
     * @brief Performs the update step of the Kalman filter using accelerometer
     * data to correct the orientation estimate.
     *
     * @param[in] accel -- The accelerometer measurements as a 3D vector,
     * typically representing the gravity vector in the sensor frame.
     */
    void updateOrientation(const Eigen::Vector3d& accel);
    /**
     * @brief Updates the airspeed estimate using a simple Kalman filter based
     * on the airspeed measurement from the sensor data.
     *
     * @param[in] raw_airspeed -- The raw airspeed measurement from the sensor
     * data.
     * @param[in] forward_accel -- The true forward acceleration -- derived from
     * the orientation and accelerometer data -- used to predict changes in
     * airspeed.
     *
     * @warning This method is slightly vulnerable to centripetal forces during
     * turns, which can cause the forward accel to be overestimated. In
     * practice, just dont use such simple math for fighter jets.
     */
    void updateAirspeed(const double raw_airspeed, const double forward_accel);
    /**
     * @brief Calculates the altitude based on the raw pressure measurement and
     * the current QNH setting, and updates the altitude estimate accordingly.
     *
     * @param[in] raw_pressure -- The raw pressure measurement from the sensor
     * data, in Pascals.
     */
    void updateAltitude(const double raw_pressure);

    /**
     * @brief Calculates the heading based on magnetometer measurements and
     * gyroscope data, and updates the heading estimate using a Kalman filter.
     *
     * The implemented filter is a simple 2D Kalman filter that estimates the
     * heading angle and its rate of change. The magnetometer provides a direct
     * measurement of the heading, while the gyroscope provides information
     * about the rate of change of the heading. The filter fuses these
     * measurements to try and provide provide a more accurate and stable
     * heading estimate.
     *
     * @param[in] mag_y -- The Y-axis magnetometer measurement, in microteslas
     * @param[in] mag_x -- The X-axis magnetometer measurement, in microteslas
     * (admittedly the unit of measurement is not important -- atan2)
     * @param[in] gyro_z -- The Z-axis gyroscope measurement, in radians per
     * second
     */
    void updateHeading(const float mag_y, const float mag_x,
                       const double gyro_z);
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
     * @brief Emitted when the airspeed estimate is updated after processing new
     * sensor data.
     *
     * @param[in] airspeed -- The updated airspeed estimate in knots.
     */
    void airspeedUpdated(double airspeed);

    /**
     * @brief Emitted when the altitude estimate is updated after processing new
     * sensor data.
     *
     * @param[in] altitude -- The updated altitude estimate in feet.
     */
    void altitudeUpdated(double altitude);
    /**
     * @brief Emitted when the heading estimate is updated after processing new
     * sensor data.
     *
     * @param[in] heading -- The updated heading estimate in degrees, normalized
     * to the range [0, 360).
     */
    void headingUpdated(double heading);
    /**
     * @brief Emitted when the temperature, GPS satellite count, or GPS fix
     * status is updated after processing new sensor data.
     *
     * @param[in] oat -- The outside air temperature (OAT) in degrees Celsius.
     * @param[in] gps_sats -- The number of GPS satellites in view.
     * @param[in] gps_fix -- The GPS fix status (0 for no fix, 1 for 2D fix, 2
     * for 3D fix).
     */
    void temperatureGPSUpdated(float oat, uint8_t gps_sats, uint8_t gps_fix);
    void GPSHeadingUpdated(double gps_lat, double gps_lon, double heading);

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
  public slots:
    /**
     * @brief Updates the QNH setting based on user input from the QNH knob in
     * the DCP.
     *
     * @param[in] qnh -- The new QNH value in Pa.
     */
    void handleQNHKnobChange(double qnh);
};
#endif // DATAFUSIONENGINE_H_
