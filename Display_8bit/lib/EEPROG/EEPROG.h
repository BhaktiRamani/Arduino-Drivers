#ifndef _EEPROG_H
#define _EEPROG_H

extern class EEPROG {
public:
  void setInit();
  void setStandby();
  void setRead();
  void setWrite();
  static unsigned char readEEPROM(unsigned int address);
  void writeEEPROM(unsigned int address, unsigned char data);
  void printContents();
  void eraseALL();
} EEPROG;

#endif
