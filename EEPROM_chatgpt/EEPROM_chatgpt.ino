#include <avr/io.h>
#include <util/delay.h>

#define SHIFT_DATA_PIN   2
#define SHIFT_CLK_PIN    3
#define SHIFT_LATCH_PIN  4
#define EEPROM_D0_PIN    5

#define EEPROM_D7_PIN    12
#define WRITE_EN_PIN     13

#define SHIFT_DATA_PORT  PORTD
#define SHIFT_DATA_DDR   DDRD
#define SHIFT_CLK_PORT   PORTD
#define SHIFT_CLK_DDR    DDRD
#define SHIFT_LATCH_PORT PORTD
#define SHIFT_LATCH_DDR  DDRD
#define EEPROM_D0_PORT   PORTD
#define EEPROM_D0_PIN    PIND
#define EEPROM_D0_DDR    DDRD
#define EEPROM_D7_PORT   PORTB
#define EEPROM_D7_PIN    PINB
#define EEPROM_D7_DDR    DDRB
#define WRITE_EN_PORT    PORTB
#define WRITE_EN_DDR     DDRB

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, uint8_t outputEnable) {
    SHIFT_DATA_PORT = (address >> 8) | (outputEnable ? 0x00 : 0x80);
    SHIFT_CLK_PORT |= (1 << SHIFT_CLK_PIN);
    SHIFT_CLK_PORT &= ~(1 << SHIFT_CLK_PIN);

    SHIFT_DATA_PORT = address;
    SHIFT_CLK_PORT |= (1 << SHIFT_CLK_PIN);
    SHIFT_CLK_PORT &= ~(1 << SHIFT_CLK_PIN);

    SHIFT_LATCH_PORT |= (1 << SHIFT_LATCH_PIN);
    SHIFT_LATCH_PORT &= ~(1 << SHIFT_LATCH_PIN);
}

/*
 * Read a byte from the EEPROM at the specified address.
 */
uint8_t readEEPROM(int address) {
    for (int pin = EEPROM_D0_PIN; pin <= EEPROM_D7_PIN; pin += 1) {
        EEPROM_D0_DDR &= ~(1 << pin);
    }
    setAddress(address, 1);

    uint8_t data = 0;
    for (int pin = EEPROM_D7_PIN; pin >= EEPROM_D0_PIN; pin -= 1) {
        data = (data << 1) | ((EEPROM_D7_PORT >> pin) & 0x01);
    }
    return data;
}

/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, uint8_t data) {
    setAddress(address, 0);
    for (int pin = EEPROM_D0_PIN; pin <= EEPROM_D7_PIN; pin += 1) {
        EEPROM_D0_DDR |= (1 << pin);
    }

    for (int pin = EEPROM_D0_PIN; pin <= EEPROM_D7_PIN; pin += 1) {
        EEPROM_D0_PORT = (EEPROM_D0_PORT & ~(1 << pin)) | ((data & 0x01) << pin);
        data >>= 1;
    }

    WRITE_EN_PORT &= ~(1 << WRITE_EN_PIN);
    _delay_us(1);
    WRITE_EN_PORT |= (1 << WRITE_EN_PIN);
    _delay_ms(12);
}

/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
    for (int base = 0; base <= 255; base += 16) {
        uint8_t data[16];
        for (int offset = 0; offset <= 15; offset += 1) {
            data[offset] = readEEPROM(base + offset);
        }

        char buf[80];
        sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
                base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
                data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

        // Print buf as needed in your bare-metal environment.
    }
}

void setup() {
    // Initialize your microcontroller's ports and peripherals here

    SHIFT_DATA_DDR |= (1 << SHIFT_DATA_PIN);
    SHIFT_CLK_DDR |= (1 << SHIFT_CLK_PIN);
    SHIFT_LATCH_DDR |= (1 << SHIFT_LATCH_PIN);
    EEPROM_D0_DDR |= (1 << EEPROM_D0_PIN);
    EEPROM_D7_DDR |= (1 << EEPROM_D7_PIN);
    WRITE_EN_DDR |= (1 << WRITE_EN_PIN);

    // Your setup code goes here

    // Erase and program EEPROM
    // for (int address = 0; address <= 2047; address += 1) {
    //     writeEEPROM(address, 0xff);
    // }
    writeEEPROM(0x5555, 0xAA);
    readEEPROM(0x5555);
    Serial.println("WRITING EEPROM");
    writeEEPROM(0x2AAA, 0x55);
    readEEPROM(0x2AAA);
    writeEEPROM(0x5555, 0x80);
    readEEPROM(0x5555);
    writeEEPROM(0x5555, 0xAA);
    readEEPROM(0x5555);
    writeEEPROM(0x2AAA, 0x55);
    readEEPROM(0x2AAA);
    writeEEPROM(0x5555, 0x20);
    readEEPROM(0x5555);

    // Read and print out the contents of the EEPROM
    printContents();
}

void loop() {
    // Your main loop code goes here
}
