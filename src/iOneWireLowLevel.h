#pragma once
#include <cinttypes>

class IOneWireLowLevel {
 public:
  virtual bool read() = 0;
  virtual void writeHigh() = 0;
  virtual void writeLow() = 0;
  virtual void setAsOutput() = 0;
  virtual void setAsInput() = 0;
  virtual uint32_t waitWhileMS(bool value, uint32_t time) = 0;
  virtual void waitMS(uint32_t time) = 0;
};
