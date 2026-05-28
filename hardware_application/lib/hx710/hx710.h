#ifndef LIB_HX710_H
#define LIB_HX710_H

#include "hardware/pio.h"
typedef struct {
    PIO pio;
    uint32_t sm;
    uint32_t clock_pin;
    uint32_t data_pin;
    uint32_t mode;

    int32_t offset;
    float scale;
} hx710_t;

void hx710_init(hx710_t* obj, PIO pio, uint sm, uint offset, uint clock_pin,
                uint data_pin, uint mode);
void hx710_set_mode(hx710_t* obj, uint mode);
bool hx710_read(hx710_t* obj, int32_t* out_result);
bool hx710_read_average(hx710_t* obj, uint times, float* out_avg);
bool hx710_tare(hx710_t* obj, uint times);

void hx710_set_scale(hx710_t* obj, float scale);
void hx710_set_offset(hx710_t* obj, int32_t offset);

void hx710_power_down(hx710_t* obj);
void hx710_power_up(hx710_t* obj);

#endif // LIB_HX710_H
