#pragma once

class IOneWireLowLevel {
 public:
  bool read();
  void writeHigh();
  void writeLow();
  void setAsOutput();
  void setAsInput();
};
