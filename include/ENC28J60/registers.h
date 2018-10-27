#ifndef ENC28J60__REGISTERS_H_
#define ENC28J60__REGISTERS_H_

namespace enc28j60 {

    enum class Register : uint8_t {
        // All-bank registers
        EIE = 0x1B,
        EIR = 0x1C,
        ESTAT = 0x1D,
        ECON2 = 0x1E,
        ECON1 = 0x1F,

        // Bank 0 registers
        ERDPT = (0x00|0x00),
        EWRPT = (0x02|0x00),
        ETXST = (0x04|0x00),
        ETXND = (0x06|0x00),
        ERXST = (0x08|0x00),
        ERXND = (0x0A|0x00),
        ERXRDPT = (0x0C|0x00),
        // #define ERXWRPT         (0x0E|0x00)
        EDMAST = (0x10|0x00),
        EDMAND = (0x12|0x00),
        // #define EDMADST         (0x14|0x00)
        EDMACS = (0x16|0x00),

        // Bank 1 registers
        EHT0 = (0x00|0x20),
        EHT1 = (0x01|0x20),
        EHT2 = (0x02|0x20),
        EHT3 = (0x03|0x20),
        EHT4 = (0x04|0x20),
        EHT5 = (0x05|0x20),
        EHT6 = (0x06|0x20),
        EHT7 = (0x07|0x20),
        EPMM0 = (0x08|0x20),
        EPMM1 = (0x09|0x20),
        EPMM2 = (0x0A|0x20),
        EPMM3 = (0x0B|0x20),
        EPMM4 = (0x0C|0x20),
        EPMM5 = (0x0D|0x20),
        EPMM6 = (0x0E|0x20),
        EPMM7 = (0x0F|0x20),
        EPMCS = (0x10|0x20),
        // #define EPMO            (0x14|0x20)
        EWOLIE = (0x16|0x20),
        EWOLIR = (0x17|0x20),
        ERXFCON = (0x18|0x20),
        EPKTCNT = (0x19|0x20),

        // Bank 2 registers
        MACON1 = (0x00|0x40|0x80),
        MACON3 = (0x02|0x40|0x80),
        MACON4 = (0x03|0x40|0x80),
        MABBIPG = (0x04|0x40|0x80),
        MAIPG = (0x06|0x40|0x80),
        MACLCON1 = (0x08|0x40|0x80),
        MACLCON2 = (0x09|0x40|0x80),
        MAMXFL = (0x0A|0x40|0x80),
        MAPHSUP = (0x0D|0x40|0x80),
        MICON = (0x11|0x40|0x80),
        MICMD = (0x12|0x40|0x80),
        MIREGADR = (0x14|0x40|0x80),
        MIWR = (0x16|0x40|0x80),
        MIRD = (0x18|0x40|0x80),

        // Bank 3 registers
        MAADR1 = (0x00|0x60|0x80),
        MAADR0 = (0x01|0x60|0x80),
        MAADR3 = (0x02|0x60|0x80),
        MAADR2 = (0x03|0x60|0x80),
        MAADR5 = (0x04|0x60|0x80),
        MAADR4 = (0x05|0x60|0x80),
        EBSTSD = (0x06|0x60),
        EBSTCON = (0x07|0x60),
        EBSTCS = (0x08|0x60),
        MISTAT = (0x0A|0x60|0x80),
        EREVID = (0x12|0x60),
        ECOCON = (0x15|0x60),
        EFLOCON = (0x17|0x60),
        EPAUS = (0x18|0x60)
    };

    enum class RegisterBank : uint8_t {
        BANK0 = 0x0,
        BANK1 = 0x20,
        BANK2 = 0x40,
        BANK3 = 0x60
    };


    // ENC28J60 ERXFCON Register Bit Definitions
    enum class ERXFCON : uint8_t {
        UCEN = 0x80,
        ANDOR = 0x40,
        CRCEN = 0x20,
        PMEN = 0x10,
        MPEN = 0x08,
        HTEN = 0x04,
        MCEN = 0x02,
        BCEN = 0x01
    };

    // ENC28J60 EIE Register Bit Definitions
    enum class EIE : uint8_t {
        INTIE = 0x80,
        PKTIE = 0x40,
        DMAIE = 0x20,
        LINKIE = 0x10,
        TXIE = 0x08,
        WOLIE = 0x04,
        TXERIE = 0x02,
        RXERIE = 0x01
    };

    // ENC28J60 EIR Register Bit Definitions
    enum class EIR : uint8_t {
        PKTIF = 0x40,
        DMAIF = 0x20,
        LINKIF = 0x10,
        TXIF = 0x08,
        WOLIF = 0x04,
        TXERIF = 0x02,
        RXERIF = 0x01
    };

    // ENC28J60 ESTAT Register Bit Definitions
    enum class ESTAT : uint8_t {
        INT = 0x80,
        LATECOL = 0x10,
        RXBUSY = 0x04,
        TXABRT = 0x02,
        CLKRDY = 0x01
    };

    // ENC28J60 ECON2 Register Bit Definitions
    enum class ECON2 : uint8_t {
        AUTOINC = 0x80,
        PKTDEC = 0x40,
        PWRSV = 0x20,
        VRPS = 0x08
    };

    // ENC28J60 ECON1 Register Bit Definitions
    enum class ECON1 : uint8_t {
        TXRST = 0x80,
        RXRST = 0x40,
        DMAST = 0x20,
        CSUMEN = 0x10,
        TXRTS = 0x08,
        RXEN = 0x04,
        BSEL1 = 0x02,
        BSEL0 = 0x01
    };

    // ENC28J60 MACON1 Register Bit Definitions
    enum class MACON1 : uint8_t {
        LOOPBK = 0x10,
        TXPAUS = 0x08,
        RXPAUS = 0x04,
        PASSALL = 0x02,
        MARXEN = 0x01
    };

    // ENC28J60 MACON3 Register Bit Definitions
    enum class MACON3 : uint8_t {
        PADCFG2 = 0x80,
        PADCFG1 = 0x40,
        PADCFG0 = 0x20,
        TXCRCEN = 0x10,
        PHDRLEN = 0x08,
        HFRMLEN = 0x04,
        FRMLNEN = 0x02,
        FULDPX = 0x01
    };

    // ENC28J60 MICMD Register Bit Definitions
    enum class MICMD : uint8_t {
        MIISCAN = 0x02,
        MIIRD = 0x01
    };

    // ENC28J60 MISTAT Register Bit Definitions
    enum class MISTAT : uint8_t {
        NVALID = 0x04,
        SCAN = 0x02,
        BUSY = 0x01
    };

    // ENC28J60 EBSTCON Register Bit Definitions
    enum class EBSTCON : uint8_t {
        PSV2 = 0x80,
        PSV1 = 0x40,
        PSV0 = 0x20,
        PSEL = 0x10,
        TMSEL1 = 0x08,
        TMSEL0 = 0x04,
        TME = 0x02,
        BISTST = 0x01
    };

    enum class EBSTCON_TESTMODE : uint8_t {
        PATTERN_SHIFT_FILL = (EBSTCON::TMSEL1),
        ADDRESS_FILL = (EBSTCON::TMSEL0),
        RANDOM_FILL = 0x0
    };

    // PHY registers
    enum class PhyRegister : uint8_t {
        CON1 = 0x00,
        STAT1 = 0x01,
        HID1 = 0x02,
        HID2 = 0x03,
        CON2 = 0x10,
        STAT2 = 0x11,
        IE = 0x12,
        IR = 0x13,
        LCON = 0x14
    };

    // ENC28J60 PHY PHCON1 Register Bit Definitions
    enum class PHCON1 : uint16_t {
        PRST = 0x8000,
        PLOOPBK = 0x4000,
        PPWRSV = 0x0800,
        PDPXMD = 0x0100
    };

    // ENC28J60 PHY PHSTAT1 Register Bit Definitions
    enum class PHSTAT1 : uint16_t {
        PFDPX = 0x1000,
        PHDPX = 0x0800,
        LLSTAT = 0x0004,
        JBSTAT = 0x0002
    };

    // PHY PHSTAT2 Register Bit Definitions
    enum class PHSTAT2 : uint16_t {
        PLRITY = (1 << 5),
        DPXSTAT = (1 << 9),
        LSTAT = (1 << 10),
        COLSTAT = (1 << 11),
        RXSTAT = (1 << 12),
        TXSTAT = (1 << 13)
    };

    // ENC28J60 PHY PHCON2 Register Bit Definitions
    enum class PHCON2 : uint16_t {
        FRCLINK = 0x4000,
        TXDIS = 0x2000,
        JABBER = 0x0400,
        HDLDIS = 0x0100
    };

}

#endif // ifndef ENC28J60__REGISTERS_H_
