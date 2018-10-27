#include <ENC28J60/ENC28J60.h>

using namespace enc28j60;

ENC28J60::ENC28J60(uint32_t spi, uint32_t cs_port, uint32_t cs_pin, const uint8_t *mac_addr) :
    _spi(spi),
    _cs_port(cs_port),
    _cs_pin(cs_pin) {
    for (int i = 0; i < mac_addr_len; i++) {
        _mac_addr[i] = mac_addr[i];
    }
}

void ENC28J60::chip_select() {
    gpio_clear(_cs_port, _cs_pin);
}

void ENC28J60::chip_deselect() {
    gpio_set(_cs_port, _cs_pin);
}

uint8_t ENC28J60::read(ReadOp op, Register reg) {
    return read(op, static_cast<uint8_t>(reg));
}

uint8_t ENC28J60::spi_xfer(uint8_t byte) {
    // Wait for TX ready
    while (!(SPI_SR(_spi) & SPI_SR_TXE));

    // Set tx data
    SPI_DR8(_spi) = byte;

    // Wait for RX
    while (!(SPI_SR(_spi) & SPI_SR_RXNE));

    // Read response
    return SPI_DR8(_spi);
}

uint8_t ENC28J60::read(ReadOp op, uint8_t addr) {
    chip_select();
    const uint8_t op_reg = static_cast<uint8_t>(op) | (addr & addr_mask);
    spi_xfer(op_reg);
    uint8_t result = spi_xfer(0x0);
    if (addr & 0x80)
        result = spi_xfer(0x0);
    chip_deselect();
    return result;
}
void ENC28J60::write(WriteOp op, Register reg, uint8_t value) {
    write(op, static_cast<uint8_t>(reg), value);
}

void ENC28J60::write(WriteOp op, uint8_t reg, uint8_t value) {
    const uint8_t op_reg = static_cast<uint8_t>(op) | (reg & addr_mask);
    chip_select();
    spi_xfer(op_reg);
    spi_xfer(value);
    chip_deselect();
}

void ENC28J60::read_buffer(uint16_t len, uint8_t *data) {
    const uint8_t op = static_cast<uint8_t>(ReadOp::READ_BUF_MEM);
    spi_xfer(op);
    for (uint16_t i = 0; i < len; i++) {
        data[i] = spi_xfer(0x0);
    }
}

void ENC28J60::write_buffer(uint16_t len, const uint8_t *data) {
    const uint8_t op = static_cast<uint8_t>(WriteOp::WRITE_BUF_MEM);
    spi_xfer(op);
    for (uint16_t i = 0; i < len; i++) {
        spi_xfer(data[i]);
    }
}

void ENC28J60::set_bank_for_register(Register addr) {
    const uint8_t bank_addr = (static_cast<uint8_t>(addr) & bank_mask) >> 5;
    if (_current_register_bank == bank_addr) {
        return;
    }
    const uint8_t clear_bitfield = \
        static_cast<uint8_t>(ECON1::BSEL1) | static_cast<uint8_t>(ECON1::BSEL0);
    write(WriteOp::BIT_FIELD_CLR, Register::ECON1, clear_bitfield);
    write(WriteOp::BIT_FIELD_SET, Register::ECON1, bank_addr);
    _current_register_bank = bank_addr;
}

uint8_t ENC28J60::read_register8(Register addr) {
    set_bank_for_register(addr);
    const uint8_t val = read(ReadOp::READ_CTRL_REG, addr);
    return val;
}

void ENC28J60::write_register8(Register addr, uint8_t value) {
    set_bank_for_register(addr);
    write(WriteOp::WRITE_CTRL_REG, addr, value);
}

uint16_t ENC28J60::read_register16(Register addr) {
    set_bank_for_register(addr);
    const uint16_t reg_low = read(ReadOp::READ_CTRL_REG, addr);
    const uint16_t reg_hi = read(ReadOp::READ_CTRL_REG, static_cast<uint8_t>(addr)+1);
    const uint16_t result = (reg_hi << 8) | reg_low;
    return result;
}

void ENC28J60::write_register16(Register addr, uint16_t value) {
    set_bank_for_register(addr);
    write(WriteOp::WRITE_CTRL_REG, addr, value & 0xFF);
    write(WriteOp::WRITE_CTRL_REG, static_cast<uint8_t>(addr)+1, value >> 8);
}

uint16_t ENC28J60::read_phy_reg(PhyRegister address) {
    write_register8(Register::MIREGADR, static_cast<uint8_t>(address));
    write_register8(Register::MICMD, static_cast<uint8_t>(MICMD::MIIRD));
    while (read_register8(Register::MISTAT) & static_cast<uint8_t>(MISTAT::BUSY));
    write_register8(Register::MICMD, 0x00);
    return read_register16(Register::MIRD);
}

void ENC28J60::write_phy_reg(PhyRegister address, uint16_t data) {
    write_register8(Register::MIREGADR, static_cast<uint8_t>(address));
    write_register16(Register::MIWR, data);
    while (read_register8(Register::MISTAT) & static_cast<uint8_t>(MISTAT::BUSY));
}

void ENC28J60::initialize() {
    // Initialize CS pin
    gpio_mode_setup(_cs_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, _cs_pin);
    chip_deselect();

    // Perform a soft reset
    write(WriteOp::SOFT_RESET, 0, static_cast<uint8_t>(WriteOp::SOFT_RESET));

    // Wait for the clock to become ready
    // Errata #2: After sending a reset command, PHY clock is
    // stopped but the ESTAT::CLKREADY bit is not cleared.
    // Workaround: Wait at least 1ms for device to be ready.
    for (uint16_t i = 0; i < 0xFFF; i++) {asm __volatile__("nop"); }
    // Poll for CLKREADY bit
    while (!read(ReadOp::READ_CTRL_REG, Register::ESTAT)
            & static_cast<uint8_t>(ESTAT::CLKRDY));

    // Set RX buffer start/end
    write_register16(Register::ERXST, rx_buffer_start);
    write_register16(Register::ERXND, rx_buffer_start);

    // Set RX buffer read pointer
    write_register16(Register::ERXRDPT, rx_buffer_end);

    // Set TX buffer start/end
    write_register16(Register::ETXST, tx_buffer_start);
    write_register16(Register::ETXND, tx_buffer_end);

    // LED control - Link and TX/RX
    const uint16_t phy_led_control = (
        static_cast<uint16_t>(LedCfg::LINK) << 8 |
        static_cast<uint16_t>(LedCfg::TX_RX) << 4 |
        static_cast<uint16_t>(LedStretch::MEDIUM) << 2 |
        1 << 1 // Enable stretch
    );
    write_phy_reg(PhyRegister::LCON, phy_led_control);

    // Receive filter control
    const uint8_t receive_filter = (
        static_cast<uint8_t>(ERXFCON::UCEN) | // Unicast filter enabled
        static_cast<uint8_t>(ERXFCON::CRCEN) | // Bad CRC filter enabled
        static_cast<uint8_t>(ERXFCON::PMEN) |  // Pattern match filter enabled
        static_cast<uint8_t>(ERXFCON::BCEN) // Broadcast filter enabled
    );
    write_register8(Register::ERXFCON, receive_filter);

    // Pattern match mask:
    // TODO: figure out meaning of this
    write_register16(Register::EPMM0, 0x303F);
    write_register16(Register::EPMCS, 0xf7f9);

    // MAC control:
    // MARXEN: Allow MAC to receive frames
    write_register8(Register::MACON1, static_cast<uint8_t>(MACON1::MARXEN));
    const uint8_t macon3_bits = (
        static_cast<uint8_t>(MACON3::PADCFG0) | // Zero-pad short frames
        static_cast<uint8_t>(MACON3::TXCRCEN) | // Enable transmit CRC
        static_cast<uint8_t>(MACON3::FRMLNEN) // Frame length check enable
    );
    write(WriteOp::BIT_FIELD_SET, Register::MACON3, macon3_bits);

    // Non-back-to-back inter-packet gap.
    // Datasheet suggests 0x0C12
    write_register16(Register::MAIPG, 0x0C12);
    // Back-to-back packet gap. Datasheet suggests 0x12.
    write_register8(Register::MABBIPG, 0x12);

    // Set maximum frame length
    write_register16(Register::MAMXFL, maximum_frame_length);

    // Set MAC address
    write_register8(Register::MAADR5, _mac_addr[0]);
    write_register8(Register::MAADR4, _mac_addr[1]);
    write_register8(Register::MAADR3, _mac_addr[2]);
    write_register8(Register::MAADR2, _mac_addr[3]);
    write_register8(Register::MAADR1, _mac_addr[4]);
    write_register8(Register::MAADR0, _mac_addr[5]);

    // Disable half-duplex loopback
    write_phy_reg(PhyRegister::CON2, static_cast<uint16_t>(PHCON2::HDLDIS));

    // Enable interrupts for packet received
    set_bank_for_register(Register::EIE);
    write(WriteOp::BIT_FIELD_SET, Register::EIE,
        static_cast<uint8_t>(EIE::INTIE) |
        static_cast<uint8_t>(EIE::PKTIE)
    );

    // Enable receive
    set_bank_for_register(Register::ECON1);
    write(WriteOp::BIT_FIELD_SET, Register::ECON1,
        static_cast<uint8_t>(ECON1::RXEN)
    );
}

uint8_t ENC28J60::get_hardware_version() {
    return read_register8(Register::EREVID) & 0b11111;
}
