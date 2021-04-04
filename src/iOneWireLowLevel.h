#pragma once

class IOneWireLowLevel {
 public:
  virtual bool read() = 0;
  virtual void writeHigh() = 0;
  virtual void writeLow() = 0;
  virtual void setAsOutput() = 0;
  virtual void setAsInput() = 0;
};
