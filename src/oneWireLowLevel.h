#pragma once
#include "oneWireTime.h"

class OneWireLowLevel {
 public:
  bool checkReset();
  bool showPresence();
  bool sendBit(bool bit);
  bool recvBit(bool& bit);

  bool readWhile(bool value, OneWireTime::timeOW_t time);
  bool readUntil(bool value, OneWireTime::timeOW_t time);
  bool read();
  void write(bool value);
  void setAsOutput();
  void setAsInput();
};