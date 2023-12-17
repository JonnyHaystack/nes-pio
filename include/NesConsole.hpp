#ifndef _NES_NES_CONSOLE_HPP
#define _NES_NES_CONSOLE_HPP

#include "nes.h"
#include "nes_definitions.h"

class NesConsole {
  public:
    /**
     * @brief Construct a new NesConsole object
     *
     * @param data_pin The GPIO pin that is connected to the NES console's data line
     * @param clock_pin The GPIO pin that is connected to the NES console's clock line
     * @param latch_pin The GPIO pin that is connected to the NES console's latch line
     * @param pio The PIO instance; either pio0 or pio1. Default is pio0.
     * @param sm The PIO state machine to run the NES program on. Default is to automatically claim
     * an unused one.
     * @param offset The instruction memory offset at which to load the nes_write PIO program.
     * Default is to allocate automatically.
     */
    NesConsole(
        uint data_pin,
        uint clock_pin,
        uint latch_pin,
        PIO pio = pio0,
        int sm = -1,
        int offset = -1
    );

    /**
     * @brief Cleanly terminate the PIO program, freeing the state machine, and uninstalling the
     * nes_write program from the PIO instance
     */
    ~NesConsole();

    /**
     * @brief Detect if a NES console is connected to the port
     *
     * @return true if console is detected, false otherwise
     */
    bool Detect();

    /**
     * @brief Send a NES controller input report to a connected NES console
     *
     * @param report The report to send
     */
    void SendReport(nes_report_t &report);

    /**
     * @brief Get the offset at which the PIO program was installed. Useful if you want to
     * communicate with multiple NES controllers without having to load multiple copies of the PIO
     * program.
     *
     * @return The offset at which the PIO program was installed
     */
    int GetOffset();

  private:
    static constexpr uint packet_size = 8;
    static uint8_t _instance_count;

    uint8_t _instance;
    nes_port_t _port;
    nes_report_t _report;
    bool _latch_triggered = false;

    static void LatchIrqHandler(uint gpio, uint32_t event_mask);
};

#endif