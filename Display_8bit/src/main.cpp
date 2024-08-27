// Programming tasks
//#define ERASE 1
#define DIGITS 1
#define DUMP 1

#include <Arduino.h>
#include <EEPROG.h>

#ifdef DIGITS
// Program the digits. Each segment is encoded through the address lines A8 and A9.
void programDigits() {
  EEPROG.setWrite();
  // Bit patterns for the digits 0..9..F
  byte intdigits[] = {0xb7, 0x14, 0x73, 0x76, 0xd4, 0xe6, 0xe7, 0x34, 0xf7, 0xf6, 0xf5, 0xc7, 0xa3, 0x57, 0xe3, 0xe1};

  // Program the unsigned integers. Switches to A10 = LOW and to A11 = LOW -> Memory addresses from 0 to 1023
  Serial.println("Programming unsigned integers ones place");
  // A8 = HIGH; A9 = HIGH -> Memory addresses from 768 to 1023
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 768, intdigits[value % 10]);
  }
  Serial.println("Programming unsigned integers tens place");
  // A8 = LOW; A9 = HIGH -> Memory addresses from 512 to 767
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 512, intdigits[(value / 10) % 10]);
  }
  Serial.println("Programming unsigned integers hundreds place");
  // A8 = HIGH; A9 = LOW -> Memory addresses from 256 to 511
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 256, intdigits[(value / 100) % 10]);
  }
  Serial.println("Programming unsigned integers sign");
  // A8 = LOW; A9 = LOW -> Memory addresses from 0 to 255
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value, 0);
  }
  // Program the twos complement integers. Switches to A10 = HIGH and to A11 = LOW -> Memory addresses from 1024 to 2047
  // A8 = HIGH; A9 = HIGH -> Memory addresses from 1792 to 2047  
  Serial.println("Programming twos complement ones place");
  for (int value = -128; value <= 127; value += 1) {
    EEPROG.writeEEPROM((byte)value + 1792, intdigits[abs(value) % 10]);
  }
  // A8 = LOW; A9 = HIGH -> Memory addresses from 1536 to 1791
  Serial.println("Programming twos complement tens place");
  for (int value = -128; value <= 127; value += 1) {
    EEPROG.writeEEPROM((byte)value + 1536, intdigits[abs(value / 10) % 10]);
  }
  // A8 = HIGH; A9 = LOW -> Memory addresses from 1280 to 1535
  Serial.println("Programming twos complement hundreds place");
  for (int value = -128; value <= 127; value += 1) {
    EEPROG.writeEEPROM((byte)value + 1280, intdigits[abs(value / 100) % 10]);
  }
  // A8 = LOW; A9 = LOW -> Memory addresses from 1024 to 1279
  Serial.println("Programming twos complement sign");
  for (int value = -128; value <= 127; value += 1) {
    if (value < 0) {
      EEPROG.writeEEPROM((byte)value + 1024, 0x40); // minus sign
    } else {
      EEPROG.writeEEPROM((byte)value + 1024, 0);
    }
  }
  // Program the unsigned hex. Switches to A10 = LOW and to A11 = HIGH -> Memory addresses from 2048 to 3071
  // A8 = HIGH; A9 = HIGH -> Memory addresses from 2816 to 3071
  Serial.println("Programming unsigned hex ones place");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 2816, intdigits[value % 16]);
  }
  // A8 = LOW; A9 = HIGH -> Memory addresses from 2560 to 2815
  Serial.println("Programming unsigned hex sixtens place");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 2560, intdigits[(value / 16) % 16]);
  }
  // A8 = HIGH; A9 = LOW -> Memory addresses from 2304 to 2559
  Serial.println("Programming unsigned hex 256hundreds place");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 2304, 0);
  }
  // A8 = LOW; A9 = LOW -> Memory addresses from 2048 to 2303
  Serial.println("Programming unsigned hex sign");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 2048, 0);
  }
  // Program the twos complement hex. Switches to A10 = HIGH and to A11 = HIGH -> Memory addresses from 3072 to 4095  
  // A8 = HIGH; A9 = HIGH -> Memory addresses from 3840 to 4095
    Serial.println("Programming twos complement hex ones place");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 3840, intdigits[value % 16]);
  }
  // A8 = LOW; A9 = HIGH -> Memory addresses from 3584 to 3839
  Serial.println("Programming twos complement hex sixtens place");
  for (unsigned int value = 0; value <= 255; value += 1) {
    EEPROG.writeEEPROM(value + 3584, intdigits[(value / 16) % 16]);
  }
  // A8 = HIGH; A9 = LOW -> Memory addresses from 3328 to 3583
  Serial.println("Programming twos complement hex 256hundreds place");
  for (int value = -128; value <= 127; value += 1) {
    if (value < 0) {
      EEPROG.writeEEPROM((byte)value + 3328, intdigits[15]); // F sign
    } else {
      EEPROG.writeEEPROM((byte)value + 3328, 0);
    }  
  }
  // A8 = LOW; A9 = LOW -> Memory addresses from 3072 to 3327
  Serial.println("Programming twos complement hex sign");
  for (int value = -128; value <= 127; value += 1) {
    if (value < 0) {
      EEPROG.writeEEPROM((byte)value + 3072, intdigits[15]); // F sign
    } else {
      EEPROG.writeEEPROM((byte)value + 3072, 0);
    }  
  }
  EEPROG.setStandby();
}

#endif

void setup() {
  Serial.begin(115200);
  EEPROG.setInit();
  Serial.print("About to program Display ROM. ");
  Serial.print("Press 'y' to continue ... ");
  while (Serial.available() <= 0);
  char answer = Serial.read();
  Serial.println(answer);
  if (answer == 'y') {
    #ifdef ERASE
    EEPROG.eraseALL();
    #endif
    
    #ifdef DIGITS
    programDigits();
    #endif
        
    #ifdef DUMP
    EEPROG.printContents();
    #endif
  }  
  Serial.println("Have a nice day.");
}

void loop() {
}