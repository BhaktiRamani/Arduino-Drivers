#include <EEPROG.h>
#include <Arduino.h>
#include <stdint.h>

// AT28C256 control lines
const unsigned int EEPROM_nWE = A0;
const unsigned int EEPROM_nOE = A1;
const unsigned int EEPROM_nCE = A2;

// 74HC595 control lines
const unsigned int SHIFT_SER = 2;
const unsigned int SHIFT_SRCLK = 3;
const unsigned int SHIFT_RCLK = 4;
const unsigned int SHIFT_nSRCLR = 5;

// Activity indicator LED
const unsigned int GREEN_LED = A3;
const unsigned int RED_LED = A4;

// Data pins (LSB to MSB)
const unsigned int dataPin[] = {6, 7, 8, 9, 10, 11, 12, 13};

// Write cycle time (ms)
const unsigned int WCT = 10;

//----------------------------------------------------------------------
// Programer Initialization
void EEPROG::setInit() {
// Pull-up resistor assignment to avoid accidental operation
  digitalWrite(EEPROM_nWE, HIGH);
  digitalWrite(EEPROM_nCE, HIGH);
  digitalWrite(EEPROM_nOE, HIGH);  

// PinMode Assignment
  pinMode(EEPROM_nCE, OUTPUT);
  pinMode(EEPROM_nOE, OUTPUT);
  pinMode(EEPROM_nWE, OUTPUT);
    
  pinMode(SHIFT_SER, OUTPUT);
  pinMode(SHIFT_SRCLK, OUTPUT);
  pinMode(SHIFT_RCLK, OUTPUT);
  pinMode(SHIFT_nSRCLR, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  setStandby();
  }
  
//----------------------------------------------------------------------
// Setup the EEPROM for reading data
void EEPROG::setRead() {
  digitalWrite(GREEN_LED, HIGH); // Set LED Status
  digitalWrite(EEPROM_nWE, HIGH); // EEPROM Write Inhibit
  digitalWrite(EEPROM_nCE, LOW); // EEPROM Enable
  digitalWrite(EEPROM_nOE, LOW); // EEPROM Output Enable   
  for (unsigned int pin = 0; pin <= 7; pin += 1) {
    pinMode(dataPin[pin], INPUT);
    }
  delay(WCT); // Write cycle time
  }
  
//----------------------------------------------------------------------
// Setup the EEPROM for writing data
void EEPROG::setWrite() {
  digitalWrite(RED_LED, HIGH); // Set LED Status
  digitalWrite(EEPROM_nWE, HIGH); // EEPROM Write Inhibit
  digitalWrite(EEPROM_nCE, LOW); // EEPROM Enable
  digitalWrite(EEPROM_nOE, HIGH); // EEPROM Output Disable    
  for (unsigned int pin = 0; pin <= 7; pin += 1) {
    pinMode(dataPin[pin], OUTPUT);
    }
  delay(WCT); // Write cycle time
  }

//----------------------------------------------------------------------
// Setup the EEPROM for Standby
void EEPROG::setStandby() {
  digitalWrite(EEPROM_nWE, HIGH); // EEPROM Write Inhibit
  digitalWrite(EEPROM_nCE, HIGH); // EEPROM Disable
  digitalWrite(EEPROM_nOE, HIGH); // EEPROM Output Disable    
  //Clear the Shift Register
  digitalWrite(SHIFT_nSRCLR, HIGH);
  digitalWrite(SHIFT_nSRCLR, LOW);
    digitalWrite(SHIFT_RCLK, LOW); //Pulse the RClock
    digitalWrite(SHIFT_RCLK, HIGH); 
    digitalWrite(SHIFT_RCLK, LOW);
  //Keep the shift register clear pin HIGH
  digitalWrite(SHIFT_nSRCLR, HIGH);
   //Clear the LED Status
 digitalWrite(GREEN_LED, LOW);
 digitalWrite(RED_LED, LOW);
 delay(WCT); // Write cycle time
  }
  
//----------------------------------------------------------------------
// Output the address bits using the shift registers.
void setAddress(unsigned int address) {
  shiftOut(SHIFT_SER, SHIFT_SRCLK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_SER, SHIFT_SRCLK, MSBFIRST, address);
  digitalWrite(SHIFT_RCLK, HIGH); // Pulse the RClock pin
  digitalWrite(SHIFT_RCLK, LOW);
  }
  
//----------------------------------------------------------------------  
// Read a byte from the EEPROM at the specified address.
uint8_t EEPROG::readEEPROM(unsigned int address) {
  setAddress(address);
  uint8_t data = 0;
  for (int pin = 7; pin >= 0; pin -= 1) {
    data = (data << 1) + digitalRead(dataPin[pin]);
    }
  return data;
  }
  
//----------------------------------------------------------------------  
// Write a byte from the EEPROM at the specified address.
void EEPROG::writeEEPROM(unsigned int address, uint8_t data) {
  setAddress(address);
  for (unsigned int pin = 0; pin <= 7; pin += 1) {
    digitalWrite(dataPin[pin],data & 1);
    data = data >> 1;
  }
  digitalWrite(EEPROM_nWE, LOW); // Pulse the WRITE pin
  digitalWrite(EEPROM_nWE, HIGH);
  delay(WCT); // Write cycle time
  }

//----------------------------------------------------------------------  
// Read the contents of the EEPROM and print them to the serial monitor.
void EEPROG::printContents() {
  Serial.println("Reading EEPROM");
  setRead();
  for (unsigned int base = 0; base <= 8191; base += 16) {
    uint16_t data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }
    char buf[60];
    sprintf(buf, "%04i:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
  setStandby();
  }
  
  //----------------------------------------------------------------------
  // Erase entire EEPROM
  void EEPROG::eraseALL() {
  Serial.print("Erasing EEPROM");
  setWrite();
  for (unsigned int address = 0; address <= 8191; address += 1) {
    writeEEPROM(address, 0xff);
    if ((address+1) % 128 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");
  setStandby();
  }