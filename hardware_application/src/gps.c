#include "gps.h"
#include "common.h"
#include "hardware/uart.h"
#include "minmea.h"
#include <hardware/gpio.h>

#define GPS_DEBUG
#define GPS_UART uart0
#define GPS_UART_TX_PIN 0
#define GPS_UART_RX_PIN 1
#define GPS_UART_BAUD_RATE 9600

void gps_init(void) {
    uart_init(GPS_UART, GPS_UART_BAUD_RATE);
    gpio_set_function(GPS_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPS_UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_fifo_enabled(GPS_UART, true);
}

bool gps_update(gps_data_t* data) {
    if (data == NULL) {
        return false;
    }
    static char rx_buf[GPS_BUFFER_SIZE] = {0};
    static size_t rx_idx = 0;
    bool new_data_available = false;
    while (uart_is_readable(GPS_UART)) {
        char c = uart_getc(GPS_UART);
#ifdef GPS_DEBUG
        print_debug("%c", c);
#endif
        if (rx_idx < GPS_BUFFER_SIZE - 1) {
            if (c == '\n') {
                rx_buf[rx_idx] = '\0';
                rx_idx = 0;
                // we have a complete NMEA sentence in rx_buf
                switch (minmea_sentence_id(rx_buf, false)) {
                case MINMEA_SENTENCE_RMC: {
                    struct minmea_sentence_rmc rmc;
                    if (minmea_parse_rmc(&rmc, rx_buf)) {
                        data->latitude = minmea_tocoord(&rmc.latitude);
                        data->longitude = minmea_tocoord(&rmc.longitude);
                        data->ground_speed = minmea_tofloat(&rmc.speed);
                        data->has_fix = rmc.valid;
                        new_data_available = true;
                    }
                    break;
                }
                case MINMEA_SENTENCE_GGA: {
                    struct minmea_sentence_gga gga;
                    if (minmea_parse_gga(&gga, rx_buf)) {
                        data->altitude = minmea_tofloat(&gga.altitude);
                        data->num_satellites = gga.satellites_tracked;
                        data->fix_quality = gga.fix_quality;
                        data->has_fix = (gga.fix_quality > 0);
                        new_data_available = true;
                    }
                    break;
                }
                default:
                    break;
                }

            } else if (c != '\r') {
                // Add character to our persistent buffer (ignoring carriage
                // returns)
                rx_buf[rx_idx++] = c;
            }

        } else {
            // Buffer overflow safety fallback
            rx_idx = 0;
        }
    }
    return new_data_available;
}
