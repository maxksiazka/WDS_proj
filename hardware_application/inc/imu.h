#ifndef IMU_H_
#define IMU_H_

#include "GY87.h"
typedef struct __attribute__((packed)){
    float x;
    float y;
    float z;
} vec3_t;
typedef struct __attribute__((packed)){
    float pressure;
    float altitude;
    float temperature;
} baro_t;
typedef struct __attribute__((packed)){
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
 * and configuring the GY-87 sensor module. It returns true if the initialization
 * was successful, and false otherwise. The function should be called before any other IMU functions to ensure that the sensors are properly set up for data
 * acquisition.
 *
 * @retval true -- initialization was successful, false otherwise (e.g., I2C communication failure, sensor initialization failure)
 * @retval false -- initialization failed (e.g., I2C communication failure, sensor initialization failure)
 */
bool imu_init(void);
void imu_read(imu_packet_t* packet);

#endif /* IMU_H_ */
