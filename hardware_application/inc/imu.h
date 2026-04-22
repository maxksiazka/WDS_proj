#ifndef IMU_H_
#define IMU_H_
/**
 * @file imu.h
 * @brief IMU module for GY-87 sensor integration
 *
 * This header file defines the interface for the IMU module, which integrates
 * the GY-87 sensor module to provide accelerometer, gyroscope, magnetometer,
 * and barometer data. The module includes functions for initialization and data
 * acquisition, as well as data structures for TCP transfer.
 */

#define enable_printing 1
#include "GY87.h"

typedef struct __attribute__((packed)) {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct __attribute__((packed)) {
    float pressure;
    float altitude;
    float temperature;
} baro_t;

typedef struct __attribute__((packed)) {
    uint32_t sync_word;
    uint64_t timestamp_us;

    vec3_t accel;
    vec3_t gyro;
    vec3_t mag;
    baro_t baro;

    uint16_t checksum;
} imu_packet_t;

extern const uint32_t IMU_SYNC_WORD;
extern gy87_t imu_gy87;
extern gy87_config_t imu_gy87_config;
/**
 * @brief Initialization of the IMU module
 *
 * This function initializes the IMU module by setting up the I2C communication
 * and configuring the GY-87 sensor module. It returns true if the
 * initialization was successful, and false otherwise. The function should be
 * called before any other IMU functions to ensure that the sensors are properly
 * set up for data acquisition.
 *
 * @retval true -- initialization was successful, false otherwise (e.g., I2C
 * communication failure, sensor initialization failure)
 * @retval false --  initialization failed (e.g., I2C communication failure,
 * sensor initialization failure)
 */
bool imu_init(void);
/**
 * @brief Read data from the IMU sensors and populate the provided packet
 * structure
 *
 * This function reads data from the GY-87 sensor module, including
 * accelerometer, gyroscope, magnetometer, and barometer readings. It populates
 * the provided `imu_packet_t` structure with the sensor data, a timestamp, and
 * a checksum for data integrity verification. The function should be called
 * after successful initialization of the IMU module to acquire sensor data for
 * processing or transmission.
 * Warning: The caller is responsible for ensuring that the `imu_packet_t` is
 * allocated, this function will overwrite the contents of the provided packet
 * structure with new sensor data.
 *
 * @param[out] packet -- pointer to an `imu_packet_t` structure
 */
void imu_read(imu_packet_t* packet);
/**
 * @brief Calculate a checksum for the given IMU packet
 *
 * @param[in] packet -- pointer to an `imu_packet_t` structure for which the checksum is to be calculated
 * @return uint16_t -- the calculated checksum value for the provided IMU packet
 */
uint16_t calculate_checksum(imu_packet_t* packet);

#endif /* IMU_H_ */
