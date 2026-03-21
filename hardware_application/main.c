#include "common.h"
#include "tcp_api_implementation.h"
#include "tcp_typedefs.h"
#include "pico/stdlib.h"

#define LED_PIN CYW43_WL_GPIO_LED_PIN

int main(void) {
    stdio_init_all();
    if (init_wifi_connection(WIFI_SSID, WIFI_PASSWORD)) {
        print_debug("Wi-Fi connection established.\n");
    } else {
        print_debug("Failed to establish Wi-Fi connection.\n");
        return false;
    }
    cyw43_arch_gpio_put(LED_PIN, 1);

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
    tcp_client->connected = true;

    while (true) {
        cyw43_arch_poll();
        sleep_ms(10);
    }

    return 0;
}
