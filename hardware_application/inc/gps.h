#ifndef GPS_H_
#define GPS_H_


#include <stdbool.h>
#include <stdint.h>
#define GPS_BUFFER_SIZE 128

typedef struct {
    float latitude;
    float longitude;
    float altitude;
    float ground_speed; // Speed over ground in knots
    int32_t num_satellites; // Number of satellites in vie
    int32_t fix_quality; // GPS fix quality (0 = invalid, 1 = GPS fix, 2 = DGPS fix)
    bool has_fix; // Indicates whether a valid GPS fix is available
} gps_data_t;
/**
 * @brief Initializes the GPS module on the UART interface.
 */
void gps_init(void);

bool gps_update(gps_data_t* data);
#endif /* GPS_H_ */
