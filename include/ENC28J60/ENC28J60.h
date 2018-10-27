#ifndef ENC28J60__ENC28J60_H_
#define ENC28J60__ENC28J60_H_

#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include <ENC28J60/registers.h>

namespace enc28j60 {

    const uint8_t addr_mask = 0x1F;
    const uint8_t bank_mask = 0x60;
    const uint8_t sprd_mask = 0x80;
    const uint8_t mac_addr_len {6};

    enum class ReadOp : uint8_t {
        READ_CTRL_REG = 0x0,
        READ_BUF_MEM = 0x3A
    };

    enum class WriteOp : uint8_t {
        WRITE_CTRL_REG = 0x40,
        WRITE_BUF_MEM = 0x7A,
        BIT_FIELD_SET = 0x80,
        BIT_FIELD_CLR = 0xA0,
        SOFT_RESET = 0xFF
    };

    enum class LedCfg : uint16_t {
        DUPLEX_AND_COLLISION = 0b1110,
        LINK_STATUS_AND_TX_RX = 0b1101,
        LINK_STATUS_AND_RX = 0b1100,
        BLINK_SLOW = 0b1011,
        BLINK_FAST = 0b1010,
        OFF = 0b1001,
        ON = 0b1000,
        TX_RX = 0b0111,
        DUPLEX = 0b0101,
        LINK = 0b0100,
        COLLISION = 0b0011,
        RX = 0b0010,
        TX = 0b0001
    };

    enum class LedStretch : uint16_t {
        LONG = 0b10, // TLSTRCH, ~140ms
        MEDIUM = 0b01, // TMSTRCH, ~70ms
        NORMAL = 0b00 // TNSTRCH, ~40ms
    };

    const uint16_t rx_buffer_start {0x0000};
    const uint16_t rx_buffer_end {0x0bff};
    const uint16_t tx_buffer_start {0x0c00};
    const uint16_t tx_buffer_end {0x11ff};
    const uint16_t maximum_frame_length {1500};

    class ENC28J60 {
        public:
            ENC28J60(uint32_t spi, uint32_t cs_port, uint32_t cs_pin, const uint8_t *mac_addr);
            void initialize();
            uint8_t get_hardware_version();

        private:
            // SPI bus
            const uint32_t _spi;

            // Chip select port/pin
            const uint32_t _cs_port;
            const uint32_t _cs_pin;

            // MAC address
            uint8_t _mac_addr[6];

            // Current register bank
            uint8_t _current_register_bank = 0xFF;

            // CS control
            void chip_select();
            void chip_deselect();

            // SPI
            uint8_t spi_xfer(uint8_t byte);

            // Basic IO operations
            uint8_t read(ReadOp op, Register reg);
            uint8_t read(ReadOp op, uint8_t reg);
            void write(WriteOp op, Register reg, uint8_t data);
            void write(WriteOp op, uint8_t reg, uint8_t data);
            // Buffer IO
            void read_buffer(uint16_t len, uint8_t *data);
            void write_buffer(uint16_t len, const uint8_t *data);

            // Register operations
            void set_bank_for_register(Register addr);
            uint8_t read_register8(Register addr);
            void write_register8(Register addr, uint8_t value);
            uint16_t read_register16(Register addr);
            void write_register16(Register addr, uint16_t value);

            // PHY
            uint16_t read_phy_reg(PhyRegister address);
            void write_phy_reg(PhyRegister address, uint16_t data);
    };

}

#endif // #ifndef ENC28J60__ENC28J6)_H_
