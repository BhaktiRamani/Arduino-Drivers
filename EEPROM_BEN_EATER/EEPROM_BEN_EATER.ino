#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WE 13
#define CE 0
#define OE 2

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  //delayMicroseconds(10);
  //setAddress(address);
  delayMicroseconds(10);
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);
  digitalWrite(CE,LOW);
  delayMicroseconds(150);
  
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  
  delayMicroseconds(10);
  digitalWrite(WE, HIGH);
  digitalWrite(CE, HIGH);
  digitalWrite(OE, HIGH);
  delayMicroseconds(10);
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  delayMicroseconds(10);
  setAddress(address);
  delayMicroseconds(10);
  digitalWrite(OE, HIGH);
  digitalWrite(WE, LOW);
  digitalWrite(CE,LOW);
  delayMicroseconds(150);
  
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
 
  delayMicroseconds(10);
  delayMicroseconds(10);
  delayMicroseconds(10);
  digitalWrite(WE, HIGH);
  digitalWrite(CE, HIGH);
  digitalWrite(OE,HIGH);
  delayMicroseconds(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
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


// 4-bit hex decoder for common anode 7-segment display
byte data[] = { 0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8 };

// 4-bit hex decoder for common cathode 7-segment display
// byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WE, HIGH);
  pinMode(WE, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(OE,OUTPUT);
  digitalWrite(WE, HIGH);
  digitalWrite(CE, HIGH);
  digitalWrite(OE, LOW);
  Serial.begin(57600);

  // Erase entire EEPROM
  // Serial.print("Erasing EEPROM");
  // for (int address = 0; address <= 2047; address += 1) {
  //   writeEEPROM(address, 0xff);

  //   if (address % 64 == 0) {
  //     Serial.print(".");
  //   }
  // }
  // Serial.println(" done");


  // Program data bytes
  // Serial.print("Programming EEPROM");
  // for (int address = 0; address < sizeof(data); address += 1) {
  //   writeEEPROM(address, data[address]);

  //   if (address % 64 == 0) {
  //     Serial.print(".");
  //   }
  // }
  // Serial.println(" done");

  //writeEEPROM(0,0x12);
  writeEEPROM(0x5555,0xAA);
  writeEEPROM(0x2aaa, 0x55);
  writeEEPROM(0x5555, 0x80);
  writeEEPROM(0x5555, 0xaa);
  writeEEPROM(0x2aaa, 0x55);
  writeEEPROM(0x5555, 0x20);
  writeEEPROM(0, 0x42);
  delay(1000);
  //Serial.println(readEEPROM(0));
  Serial.println("--------");


  // // Read and print out the contents of the EERPROM
  // Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}