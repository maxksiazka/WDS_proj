#include "imu.h"
#include "hardware/i2c.h"
#include <hardware/gpio.h>
#include <stdbool.h>
gy87_t imu_gy87={0};
const uint32_t imu_sync_word = 0xDEADBEEF;
static int32_t pico_i2c_read(void* ctx, uint8_t addr, uint8_t* data, size_t len,
                      bool nostop) {
    i2c_inst_t* i2c = (i2c_inst_t*)ctx;
    int32_t result = i2c_read_blocking(i2c, addr, data, len, nostop);
    return result;
}
static int32_t pico_i2c_write(void* ctx, uint8_t addr, const uint8_t* data, size_t len,
                       bool nostop) {
    i2c_inst_t* i2c = (i2c_inst_t*)ctx;
    int32_t result = i2c_write_blocking(i2c, addr, data, len, nostop);
    return result;
}
static uint32_t pico_get_time_ms() {
    return to_ms_since_boot(get_absolute_time());
}
static uint16_t calculate_checksum(imu_packet_t* packet) {
    uint16_t checksum = 0;
    uint8_t* data_ptr = (uint8_t*)packet;
    for (size_t i = 0; i < sizeof(imu_packet_t) - sizeof(uint16_t); i++) {
        checksum += data_ptr[i];
    }
    return checksum;
}
gy87_config_t imu_gy87_config = {
    .bmp180_addr = BMP180_ADDR,
    .hmc5883l_addr = HMC5883L_ADDR,
    .mpu6050_addr = MPU6050_ADDR,
    .ctx = i2c0,
    .delay_ms = sleep_ms,
    .get_time_ms = pico_get_time_ms,
    .i2c_read = pico_i2c_read,
    .i2c_write = pico_i2c_write,
};
bool imu_init(void) {
    i2c_init(i2c0, 400000); //400kHz
    gpio_init(20);
    gpio_init(21);

    gpio_set_function(20, GPIO_FUNC_I2C);
    gpio_set_function(21, GPIO_FUNC_I2C);

    gpio_pull_up(20);
    gpio_pull_up(21);

    imu_gy87 = gy87_init(&imu_gy87_config);
    return imu_gy87.last_read_time_ms != UINT32_MAX;
}

void imu_read(imu_packet_t* data){
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
    data->checksum = calculate_checksum(data);
}
