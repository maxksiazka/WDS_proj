#include "common.h"
#include "imu.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "tcp_api_implementation.h"
#include "tcp_typedefs.h"
#include "udp_discovery.h"

bool imu_task(){
    if (!imu_init()) {
        print_debug("Failed to initialize IMU.\n");
        return false;
    }
    return true;
}

int main(void) {
    stdio_init_all();
    cyw43_arch_init();
    print_debug("Starting up...\n");
    cyw43_arch_gpio_put(LED_PIN, 1);

    if (!imu_task()) {
        print_debug("IMU task failed to initialize.\n");
        return 1;
    }
    imu_packet_t imu_data;
    while (true) {
        imu_read(&imu_data);

        sleep_ms(500);
    }

    return 0;
}
/*
    if (!init_wifi_connection(WIFI_SSID, WIFI_PASSWORD)) {
        print_debug("Failed to establish Wi-Fi connection.\n");
        return false;
    }
    print_debug("Wi-Fi connection established.\n");
    cyw43_arch_gpio_put(LED_PIN, 1);

    while (g_connection_mgr.state != STATE_IP_DISCOVERED) {
        printf("Waiting for IP discovery...\n");
        cyw43_arch_poll();
        sleep_ms(1000);
    }

    TCP_CLIENT_T* tcp_client = tcp_client_init();
    if (tcp_client == NULL) {
        print_debug("Failed to initialize TCP client.\n");

        return false;
    }
    if (!tcp_client_open_connection(tcp_client)) {
        print_debug("Failed to open TCP connection.\n");
        tcp_client_close(tcp_client);
        return false;
    }
    tcp_client->connected = true; */
