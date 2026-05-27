#include "common.h"
#include "gps.h"
#include "imu.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "tcp_api_implementation.h"
#include "tcp_handling.h"
#include "tcp_typedefs.h"
#include "udp_discovery.h"
#include <math.h>

static volatile bool tx_timer_flag = false;
static data_packet_t data_packet = {0};

bool repeating_timer_callback(struct repeating_timer* t) {
    tx_timer_flag = true;
    return true;
}

void update_sensor_data(void) {
    imu_packet_t imu_data = {0};
    gps_data_t gps_data = {0};
    imu_read(&imu_data);
    gps_update(&gps_data);
    data_packet.sync_word = 0xDEADBEEF;
    data_packet.timestamp_us = time_us_64();
    data_packet.accel[0] = -imu_data.accel.x;
    data_packet.accel[1] = imu_data.accel.y;
    data_packet.accel[2] = imu_data.accel.z;
    data_packet.gyro[0] = imu_data.gyro.x * (M_PI / 180.0f);
    data_packet.gyro[1] = -imu_data.gyro.y * (M_PI / 180.0f);
    data_packet.gyro[2] = imu_data.gyro.z * (M_PI / 180.0f);
    data_packet.mag[0] = imu_data.mag.x;
    data_packet.mag[1] = imu_data.mag.y;
    data_packet.mag[2] = imu_data.mag.z;
    data_packet.pressure = imu_data.baro.pressure;
    data_packet.temperature = imu_data.baro.temperature;
    data_packet.altitude = imu_data.baro.altitude;
    data_packet.airspeed =
        0.0f;
    data_packet.gps_ground_speed = gps_data.ground_speed;
    data_packet.gps_lat = gps_data.latitude;
    data_packet.gps_lon = gps_data.longitude;
    data_packet.gps_sats = gps_data.num_satellites;
    data_packet.gps_fix = gps_data.fix_quality;
    data_packet.checksum = calculate_crc_ccitt(&data_packet, 0xFFFF);
}
int main(void) {
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    print_debug("Starting up...\n");
    cyw43_arch_gpio_put(LED_PIN, 1);
    TCP_CLIENT_T* tcp_client = NULL;
    print_debug("Wifi start\n");
    err_t err = init_wifi_connection(WIFI_SSID, WIFI_PASSWORD);
    if (err != ERR_OK) {
        print_debug("Failed to initialize Wi-Fi connection: main() -- %d \n", err);
        return -1;
    }
    sleep_ms(500);
    gps_init();
    imu_init();
    udp_discovery_init();
    struct repeating_timer tx_timer;
    add_repeating_timer_ms(-20, repeating_timer_callback, NULL, &tx_timer);
    while (true) {
        update_sensor_data();
        switch (g_connection_mgr.state) {
        case STATE_IDLE:
            break;
        case STATE_IP_DISCOVERED:
            tcp_client = tcp_client_init();
            if (tcp_client != NULL){
                if (!tcp_client_open_connection(tcp_client)) {
                    print_debug("Failed to open TCP connection.\n");
                    tcp_client_close(tcp_client);
                    tcp_client = NULL;
                }
            }
            g_connection_mgr.state = STATE_TCP_CONNECTING;
            break;
        case STATE_TCP_CONNECTING:
            if (tcp_client != NULL && tcp_client->connected) {
                print_debug("TCP connection established.\n");
                g_connection_mgr.state = STATE_TCP_CONNECTED;
            }
            break;
        case STATE_TCP_CONNECTED:
            if (tcp_client == NULL || !tcp_client->connected) {
                print_debug("TCP client is NULL or not connected.\n");
                g_connection_mgr.state = STATE_DISCONNECTED;
                break;
            }
            if (tx_timer_flag){
                tx_timer_flag = false;
                tcp_send_message(tcp_client, &data_packet);
            }
            break;
        case STATE_DISCONNECTED:
            print_debug("Disconnected from server. Cleaning up TCP client.\n");
            if (tcp_client != NULL) {
                tcp_client_close(tcp_client);
                tcp_client = NULL;
            }
            g_connection_mgr.state = STATE_IDLE;
            break;
        }
        cyw43_arch_poll();
    }
    return 0;
}
