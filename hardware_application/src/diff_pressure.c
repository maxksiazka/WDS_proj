#include "diff_pressure.h"
#include "hx710.h"
#include "hx710.pio.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include <pico/time.h>
#include <stdio.h>
#define CLOCK_PIN 17
#define DATA_PIN 16
#define PASCAL_SCALE_FACTOR 150.0f
static hx710_t sensor;
volatile bool new_data_available = false;
volatile float latest_pressure_pa = 0.0f;

static void hx710_fifo_isr(void) {
    int32_t out_result = 0;
    if (hx710_read(&sensor, &out_result)) {
        latest_pressure_pa = (float)(out_result - sensor.offset) / sensor.scale;
        new_data_available = true;
    }
    pio_sm_put(sensor.pio, sensor.sm, sensor.mode + 24);
}

void diff_pressure_init() {
    PIO pio = pio0;
    uint32_t sm = 0;
    uint32_t offset = pio_add_program(pio, &hx710_program);

    hx710_init(&sensor, pio, sm, offset, CLOCK_PIN, DATA_PIN, 1);
    hx710_set_scale(&sensor, PASCAL_SCALE_FACTOR);
    pio_set_irq0_source_enabled(pio,pis_sm0_rx_fifo_not_empty, true);
    irq_set_exclusive_handler(PIO0_IRQ_0, hx710_fifo_isr);
    irq_set_enabled(PIO0_IRQ_0, true);

    pio_sm_set_enabled(sensor.pio, sensor.sm, true);
    pio_sm_put(sensor.pio, sensor.sm, sensor.mode + 24);
    int32_t timer = 0;

    while (timer < 30) {
        if (new_data_available) {
            printf("Latest Pressure: %.2f Pa\n", latest_pressure_pa);
            new_data_available = false;
            timer++;
            sleep_ms(1000);
        }
    }
    irq_set_enabled(PIO0_IRQ_0, false);
    hx710_tare(&sensor, 50);
    pio_sm_set_enabled(sensor.pio, sensor.sm, true);
    pio_sm_put(sensor.pio, sensor.sm, sensor.mode + 24);
    irq_set_enabled(PIO0_IRQ_0, true);
}
