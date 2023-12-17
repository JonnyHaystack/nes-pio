#ifndef _NES_NES_H
#define _NES_NES_H

#include <hardware/pio.h>
#include <pico/stdlib.h>

/**
 * @brief A structure representing a Joybus instance on a given GPIO pin
 */
typedef struct {
    uint data_pin;
    uint clock_pin;
    uint latch_pin;
    uint packet_size;
    PIO pio;
    uint sm;
    uint offset;
    pio_sm_config config;
} nes_port_t;

#ifdef __cplusplus
extern "C" {
#endif

/* DEVICE MODE INTERFACE */

/**
 * @brief Initialize the nes_write PIO program and populate necessary information in a struct
 *
 * @param port Pointer to the port structure to initialize
 * @param data_pin The pin to use for shifting data out to the console
 * @param clock_pin The pin to check for clock signal on
 * @param latch_pin The pin to check for latch signal on
 * @param packet_size The number of bits per data packet
 * @param pio The PIO instance; either pio0 or pio1
 * @param sm The state machine to run the PIO program on. Pass in -1 to automatically claim
 * unused.
 * @param offset The instruction memory offset at which to load the program. Pass in -1 to allocate
 * automatically.
 *
 * @return The offset at which the nes_write PIO program is loaded
 */
uint nes_device_port_init(
    nes_port_t *port,
    uint data_pin,
    uint clock_pin,
    uint latch_pin,
    uint packet_size,
    PIO pio,
    int sm,
    int offset
);

/**
 * @brief Cleanly terminate the PIO program, freeing the state machine, and uninstalling the
 * nes_write program from the PIO instance
 *
 * @param port Pointer to the port to terminate
 */
void nes_device_port_terminate(nes_port_t *port);

/**
 * @brief Send an individual byte to a joybus device/host
 *
 * @param port The NES instance to send the byte to
 * @param data The data packet to send
 */
void nes_device_send_packet(nes_port_t *port, uint16_t data);

/* HOST MODE INTERFACE */

/**
 * @brief Initialize the nes_read PIO program and populate necessary information in a struct
 *
 * @param port Pointer to the port structure to initialize
 * @param data_pin The pin to use for reading data from the controller
 * @param clock_pin The pin to send clock pulses on
 * @param latch_pin The pin to send latch pulses on
 * @param pio The PIO instance; either pio0 or pio1
 * @param sm The state machine to run the PIO program on. Pass in -1 to automatically claim
 * unused.
 * @param offset The instruction memory offset at which to load the program. Pass in -1 to allocate
 * automatically.
 *
 * @return The offset at which the nes_read PIO program is loaded
 */
void nes_host_port_init(
    nes_port_t *port,
    uint data_pin,
    uint clock_pin,
    uint latch_pin,
    uint packet_size,
    PIO pio,
    int sm,
    int offset
);

/**
 * @brief Cleanly terminate the PIO program, freeing the state machine, and uninstalling the
 * nes_write program from the PIO instance
 *
 * @param port Pointer to the port to terminate
 */
void nes_host_port_terminate(nes_port_t *port);

/**
 * @brief Reset the state machine to clean state ready to start receiving data from a connected
 * NES device/host
 *
 * @param port The NES port to reset
 */
void nes_host_port_reset(nes_port_t *port);

/**
 * @brief Receive a data packet from a NES controller
 *
 * @param port The NES port to receive the packet on
 *
 * @return The received data packet
 */
uint16_t nes_host_receive_packet(nes_port_t *port);

#ifdef __cplusplus
}
#endif

#endif