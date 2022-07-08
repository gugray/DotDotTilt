#ifndef DOGM204_H
#define DOGM204_H

// https://www.mikrocontroller.net/topic/449286

class DOGM204
{
private:
  const uint8_t resetPin;

private:
  void writeIns(uint8_t ins);
  void writeByte(uint8_t data);
  void writeBytes(const char *data, uint8_t len);
  void checkBusy();

public:
  DOGM204(uint8_t resetPin);
  void reset();
  void init();
  
  // Set contrast (0-63)
  void setContrast(uint8_t contr);
};

#endif
