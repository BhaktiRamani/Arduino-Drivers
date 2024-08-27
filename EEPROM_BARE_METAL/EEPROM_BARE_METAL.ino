#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4

#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setAddress(int address, bool outputEnable);
byte readEEPROM(int address);
void writeEEPROM(int address, byte data);

void setup() {
  DDRD = (1 << 3) | (1 << 4) | (1 << 5)|(1<<2);  //setting pin D3,D4,D5 as output
  DDRB = (1 << 13);                       //setting pin B13 (OUTPUT_ENABLE) as output
  Serial.begin(57600);
  Serial.println("reading from EEPROM");
  printContents();
}


void loop() {
}

// void setAddress(int address, bool outputEnable) {
//   // Set the highest bit as the output enable bit (active low)
//   if (outputEnable) {
//     address &= ~0x8000;  //if outputEnable true then pin QH (2nd shift register) should be low (reading the data) bcz it is active low
//   } else {
//     address |= 0x8000;  //if not then high (means output enable is 1 and we are writing data)
//   }
//   byte dataMask = 0x04;
//   byte clkMask = 0x08;
//   byte latchMask = 0x10;

//   // Make sure the clock is low to start.
//   PORTD &= ~clkMask;

//   // Shift 16 bits in, starting with the MSB.
//   for (uint16_t ix = 0; (ix < 16); ix++) {
//     // Set the data bit
//     if (address & 0x8000) {
//       PORTD |= dataMask;
//     } else {
//       PORTD &= ~dataMask;
//     }

//     // Toggling the clock high then low
//     PORTD |= clkMask;
//     delayMicroseconds(3);
//     PORTD &= ~clkMask;
//     address <<= 1;
//   }

//   // Latch the shift register contents into the output register.
//   PORTD &= ~latchMask;
//   delayMicroseconds(1);
//   PORTD |= latchMask;
//   delayMicroseconds(1);
//   PORTD &= ~latchMask;
// }

void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  byte latchMask = 0x10;
  PORTD &= ~latchMask;
  delayMicroseconds(1);
  PORTD |= latchMask;
  delayMicroseconds(1);
  PORTD &= ~latchMask;

//   digitalWrite(SHIFT_LATCH, LOW);
//   digitalWrite(SHIFT_LATCH, HIGH);
//   digitalWrite(SHIFT_LATCH, LOW);
//
}


void setAddress(const uint16_t address) {
  static byte lastHigh = 0xFF;

  // shift low, and optionally high, word(s) into registers
  byte high = address >> 8;
  uint16_t bit = (lastHigh - high) ? 0x8000 : 0x80;  // Select all 16 bits, or just lower 8
  while (bit) {
    PORTC = (PORTC & B11110111) | (((address & bit) == bit) ? B00011000 : B00010000); // set dataPin and toggle clockPin high
    PORTC &= B11101111; // toggle clockPin low
    bit = bit >> 1;
  }

  // Latch the new values
  if (high != lastHigh) {
    PORTC |= B00100100; // Set latchPinHi & latchPinLo to high
    PORTC &= B11011011; // Set latchPinHi & latchPinLo to low
    lastHigh = high;
  } else {
    PORTC |= B00100000; // Set latchPinLo to high
    PORTC &= B11011111; // Set latchPinLo to low
  }
}

byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    if (pin >= 5 && pin <= 7) {
      DDRD &= ~(1 << pin);                //setting input from pin D0 to D7, portD and portB
    } else if (pin >= 8 && pin <= 12) 
    {
      DDRB &= ~(1 << (pin - 8));
    }
    //pinMode(pin, INPUT);
  }

  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    if (pin >= 5 && pin <= 7) {
      data = (data << 1) + ((PIND & (1 << pin)) >> pin);  //digitalRead(pin);
    } else if (pin >= 8 && pin <= 12) {
      data = (data << 1) + ((PINB & (1 << pin)) >> pin);
    }
  }
    return data;
    Serial.print(data);
}




void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    if (pin >= 5 && pin <= 7) {
      DDRD |= (1 << pin);  //setting output from pin D0 to D7, portD and portB
    } 
    else if (pin >= 8 && pin <= 12) {
      DDRB |= (1 << (pin - 8));
    }
  }
    // for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    //   pinMode(pin, OUTPUT);
    //
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    if (pin >= 5 && pin <= 7) {
      PORTD = (PORTD & ~(1 << pin)) | ((data & 1) << pin);
    }
    else if (pin >= 8 && pin <= 12) {
      PORTB = (PORTB & ~(1 << (pin - 8))) | ((data & 1) << (pin - 8));
    }
    data >>= 1;
  }
    //digitalWrite(pin, data & 1);
    //data = data >> 1;
}


void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}