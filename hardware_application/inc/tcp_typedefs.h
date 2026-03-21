#ifndef TCP_TYPEDEFS_H
#define TCP_TYPEDEFS_H

#ifndef WIFI_SSID
#define WIFI_SSID ""
#warning You should define WIFI_SSID in CMakeLists.txt, using -DWIFI_SSID="your_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#warning You should define WIFI_PASSWORD in CMakeLists.txt, using -DWIFI_PASSWORD="your_password"
#endif

#ifndef TCP_SERVER_PORT
#define TCP_SERVER_PORT 5001
#endif

#ifndef TCP_BUF_SIZE
#define TCP_BUF_SIZE 2048
#endif

#include <lwip/tcp.h>



extern bool ECHO_MODE_ENABLED;
typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb* tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[TCP_BUF_SIZE];
    int32_t buffer_len;
    bool connected;
} TCP_CLIENT_T;

#endif // TCP_TYPEDEFS_H

/**
 * COMMAND_STOP
 * COMMAND_FORWARD
 * COMMAND_BACKWARDS
 * COMMAND_LEFT
 * COMMAND_RIGHT
 * ...
 */
