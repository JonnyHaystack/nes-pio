#include "nes.h"

#include "nes.pio.h"

/* DEVICE MODE IMPLEMENTATION */

uint nes_device_port_init(
    nes_port_t *port,
    uint data_pin,
    uint clock_pin,
    uint latch_pin,
    uint packet_size,
    PIO pio,
    int sm,
    int offset
) {
    if (sm < 0) {
        sm = pio_claim_unused_sm(pio, true);
    } else {
        pio_sm_claim(pio, sm);
    }

    if (offset < 0) {
        offset = pio_add_program(pio, &nes_write_program);
    }

    port->data_pin = data_pin;
    port->clock_pin = clock_pin;
    port->latch_pin = latch_pin;
    port->pio = pio;
    port->sm = sm;
    port->offset = offset;
    port->config = nes_write_program_get_config(pio, sm, offset, data_pin, clock_pin, packet_size);

    return offset;
}

void nes_device_port_terminate(nes_port_t *port) {
    pio_sm_set_enabled(port->pio, port->sm, false);
    pio_sm_unclaim(port->pio, port->sm);
    pio_remove_program(port->pio, &nes_write_program, port->offset);
}

void nes_device_send_packet(nes_port_t *port, volatile uint16_t data) {
    nes_write_program_init(
        port->pio,
        port->sm,
        port->offset,
        port->data_pin,
        port->clock_pin,
        &port->config
    );

    pio_sm_put(port->pio, port->sm, ~data);
}

/* HOST MODE IMPLEMENTATION */

// TODO