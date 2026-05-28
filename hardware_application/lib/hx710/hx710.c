#include "hx710.h"
#include "hx710.pio.h" // Generated automatically by CMake
#include <pico/time.h>

void hx710_init(hx710_t* obj, PIO pio, uint sm, uint offset, uint clock_pin,
                uint data_pin, uint mode) {
    obj->pio = pio;
    obj->sm = sm;
    obj->clock_pin = clock_pin;
    obj->data_pin = data_pin;

    obj->offset = 0;
    obj->scale = 1.0f;

    hx710_program_init(pio, sm, offset, clock_pin, data_pin);
    hx710_set_mode(obj, mode);
}

void hx710_set_mode(hx710_t* obj, uint mode) {
    if (mode >= 1 && mode <= 3) {
        obj->mode = mode;
    }
    int32_t dummy;
    hx710_read(obj, &dummy);
    if (hx710_read(obj, &dummy)) {
        (void)dummy;
    }
}

bool hx710_read(hx710_t* obj, int32_t* out_result) {

    uint32_t raw = pio_sm_get(obj->pio, obj->sm);
    pio_sm_put(obj->pio, obj->sm, obj->mode + 24);

    // Discard GAIN bits via right shift
    raw >>= obj->mode;

    if (raw == 0x7FFFFFFF) {
        return false; // Sensor not responding error
    }

    int32_t result = (int32_t)raw;
    // 24-bit Sign Extension handling
    if (result > 0x7FFFFF) {
        result -= 0x1000000;
    }

    *out_result = result;
    return true;
}

bool hx710_read_average(hx710_t* obj, uint times, float* out_avg) {
    double sum = 0;

    pio_sm_set_enabled(obj->pio, obj->sm, true);
    pio_sm_put(obj->pio, obj->sm, obj->mode + 24);
    for (uint i = 0; i < times; i++) {
        int32_t val;
        if (pio_sm_is_rx_fifo_empty(obj->pio, obj->sm)) {
            i--;
        } else if (hx710_read(obj, &val)) {
            sum += val;
        }
    }
    pio_sm_set_enabled(obj->pio, obj->sm, false);
    *out_avg = (float)(sum / times);
    return true;
}


bool hx710_tare(hx710_t* obj, uint times) {
    float avg;
    if (!hx710_read_average(obj, times, &avg))
        return false;
    obj->offset = (int32_t)avg;
    return true;
}

void hx710_set_scale(hx710_t* obj, float scale) {
    obj->scale = scale;
}

void hx710_set_offset(hx710_t* obj, int32_t offset) {
    obj->offset = offset;
}


void hx710_power_down(hx710_t* obj) {
    pio_sm_restart(obj->pio, obj->sm);
    pio_sm_put(obj->pio, obj->sm, 0);
    pio_sm_set_enabled(obj->pio, obj->sm, true);
    sleep_ms(1);
    pio_sm_set_enabled(obj->pio, obj->sm, false);
}

void hx710_power_up(hx710_t* obj) {
    pio_sm_restart(obj->pio, obj->sm);
    pio_sm_set_enabled(obj->pio, obj->sm, true);
    pio_sm_set_enabled(obj->pio, obj->sm, false);
}
