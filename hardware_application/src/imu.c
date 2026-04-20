#include "imu.h"
#include "common.h"
#include "hardware/i2c.h"
#include <hardware/gpio.h>
#include <stdbool.h>
gy87_t imu_gy87 = {0};
const uint32_t IMU_SYNC_WORD = 0xDEADBEEF;

/**
 * @brief Pico-specific I2C read function that matches the expected signature
 * for the GY-87 driver.
 *
 * @param[in] ctx -- Context pointer, here it is an I2C instance (e.g., i2c0)
 * @param[in] addr -- I2C address of the target device
 * @param[out] data -- Pointer to the buffer where the read data will be stored
 * @param[in] len -- Number of bytes to read
 * @param[in] nostop -- Whether to send a stop condition after the read
 * @retval int32_t -- Number of bytes read on success, negative value on failure
 */
static int32_t pico_i2c_read(void* ctx, uint8_t addr, uint8_t* data, size_t len,
                             bool nostop);
/**
 * @brief Pico-specific I2C write function that matches the expected signature
 * for the GY-87 driver.
 *
 * @param[in] ctx -- Context pointer, here it is an I2C instance (e.g., i2c0)
 * @param[in] addr -- I2C address of the target device
 * @param[in] data -- Pointer to the buffer containing the data to be written
 * @param[in] len -- Number of bytes to write
 * @param[in] nostop -- Whether to send a stop condition after the write
 * @retval int32_t -- Number of bytes written on success, negative value on
 * failure
 */
static int32_t pico_i2c_write(void* ctx, uint8_t addr, const uint8_t* data,
                              size_t len, bool nostop);
/**
 * @brief Pico-specific function to get the current time in milliseconds since
 * boot, matching the expected signature for the GY-87 driver.
 *
 * @retval uint32_t -- Current time in milliseconds since boot
 *
 */
static uint32_t pico_get_time_ms(void);
/**
 * @brief CRC16 checksum calculation function.
 *
 * This function calculates the CRC16 checksum for the given IMU data packet. It
 * processes all fields of the packet except for the checksum itself, using the
 * CRC-16-CCITT polynomial (0x1021). The resulting checksum is returned as a
 * 16-bit unsigned integer.
 *
 * @param[in] packet -- Pointer to the IMU data packet for which the checksum is to be calculated
 * @param[in] initial_value -- Initial value for the checksum calculation (can be used for chaining checksums)
 * @retval uint16_t -- Calculated CRC16 checksum for the given IMU data packet
 */
static uint16_t calculate_checksum(imu_packet_t* packet,
                                   uint16_t initial_value);
static void pico_sleep_ms(unsigned int ms);
gy87_config_t imu_gy87_config = {
    .bmp180_addr = BMP180_ADDR,
    .hmc5883l_addr = HMC5883L_ADDR,
    .mpu6050_addr = MPU6050_ADDR,
    .ctx = i2c0,
    .delay_ms = pico_sleep_ms,
    .get_time_ms = pico_get_time_ms,
    .i2c_read = pico_i2c_read,
    .i2c_write = pico_i2c_write,
};
bool imu_init(void) {
    i2c_init(i2c0, 400000); // 400kHz
    gpio_init(20);
    gpio_init(21);

    gpio_set_function(20, GPIO_FUNC_I2C);
    gpio_set_function(21, GPIO_FUNC_I2C);

    gpio_pull_up(20);
    gpio_pull_up(21);

    imu_gy87 = gy87_init(&imu_gy87_config);
    return imu_gy87.last_read_time_ms != UINT32_MAX;
}

void imu_read(imu_packet_t* data) {
    gy87_read(&imu_gy87);
    data->sync_word = IMU_SYNC_WORD;
    data->timestamp_us = time_us_64();
    data->accel.x = imu_gy87.accel.x;
    data->accel.y = imu_gy87.accel.y;
    data->accel.z = imu_gy87.accel.z;
    data->gyro.x = imu_gy87.gyro.x;
    data->gyro.y = imu_gy87.gyro.y;
    data->gyro.z = imu_gy87.gyro.z;
    data->mag.x = imu_gy87.mag.x;
    data->mag.y = imu_gy87.mag.y;
    data->mag.z = imu_gy87.mag.z;
    data->baro.pressure = imu_gy87.pressure;
    data->baro.temperature = imu_gy87.temperature;
    data->baro.altitude = imu_gy87.altitude;
    data->checksum = calculate_checksum(data, 0);
#if imu_enable_printing
    print_debug("IMU Data: Accel(%.2f, %.2f, %.2f) Gyro(%.2f, %.2f, %.2f) "
                "Mag(%.2f, %.2f, %.2f) Baro(Pressure: %.2f hPa, Temp: %.2f "
                "C, Alt: %.2f m)\n",
                data->accel.x, data->accel.y, data->accel.z, data->gyro.x,
                data->gyro.y, data->gyro.z, data->mag.x, data->mag.y,
                data->mag.z,
                data->baro.pressure / 100.0f, // Convert to hPa
                data->baro.temperature, data->baro.altitude);
#endif
}

static int32_t pico_i2c_read(void* ctx, uint8_t addr, uint8_t* data, size_t len,
                             bool nostop) {
    i2c_inst_t* i2c = (i2c_inst_t*)ctx;
    int32_t result = i2c_read_blocking(i2c, addr, data, len, nostop);
    return result;
}
static int32_t pico_i2c_write(void* ctx, uint8_t addr, const uint8_t* data,
                              size_t len, bool nostop) {
    i2c_inst_t* i2c = (i2c_inst_t*)ctx;
    int32_t result = i2c_write_blocking(i2c, addr, data, len, nostop);
    return result;
}
static uint32_t pico_get_time_ms() {
    return to_ms_since_boot(get_absolute_time());
}
static uint16_t calculate_checksum(imu_packet_t* packet,
                                   uint16_t initial_value) {
    uint16_t res_crc = 0;
    uint8_t* data_ptr = (uint8_t*)packet;
    uint32_t data_length = sizeof(imu_packet_t) - sizeof(packet->checksum);

    // CRC16
    while (data_length--) {
        res_crc ^= *data_ptr++;
        for (int16_t i = 0; i < 8; i++) {
            res_crc = (res_crc & 0x8000) ? (res_crc << 1) ^ GENERATOR_POLYNOMIAL
                                         : (res_crc << 1);
        }
    }
    return res_crc & 0xFFFF;
}
static void pico_sleep_ms(unsigned int ms) {
    sleep_ms(ms);
}
