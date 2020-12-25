#pragma once
#include "oneWireTime.h"

class OneWireLowLevel {
 public:
 OneWireLowLevel(uint32_t pin);
  bool read();
  void writeHigh();
  void writeLow();
  void setAsOutput();
  void setAsInput();
  private:
  const uint32_t pin_;
  const uint32_t pinModeMaskInput_;
  const uint32_t pinModeMaskOutput_;
};