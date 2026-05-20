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
/**
 * struct TCP_CLIENT_T_ - TCP client structure to hold connection information and buffer
 */
typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb* tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[TCP_BUF_SIZE];
    int32_t buffer_len;
    bool connected;
} TCP_CLIENT_T;

typedef struct __attribute__((packed)) {
    uint32_t sync_word;     // Słowo synchronizacyjne (0xDEADBEEF)
    uint64_t timestamp_us;  // Czas systemowy w mikrosekundach
    float accel[3];         // Dane akcelerometru (X, Y, Z) [m/s^2]
    float gyro[3];          // Dane żyroskopu (X, Y, Z) [°/s]
    float mag[3];           // Dane magnetometru (X, Y, Z) [µT]
    float pressure;         // Ciśnienie atmosferyczne [Pa]
    float temperature;      // Temperatura [°C]
    float altitude;         // Wysokość barometryczna n.p.m. [m]
    float airspeed;         // Prędkość względem powietrza [m/s]
    float gps_ground_speed; // Prędkość względem ziemi z GPS [knots]
    float gps_lat;        // Szerokość geograficzna (°)
    float gps_lon;        // Długość geograficzna (°)
    uint8_t gps_sats;       // Liczba widocznych satelitów GPS
    uint8_t gps_fix;        // Status fiksa GPS (0 = brak, 3 = 3D)
    uint16_t checksum;      // Suma kontrolna CRC-16-CCITT (obliczana od początku do gps_fix)
} data_packet_t;
#endif // TCP_TYPEDEFS_H
