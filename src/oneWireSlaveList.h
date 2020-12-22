#pragma once
#include <oneWireSlave.h>

class OneWireSlaveList {
 public:
  OneWireSlave* create(uint8_t address[8]);
  OneWireSlave* getFirst();
  bool isSingle();
  OneWireSlave* find(uint8_t address[8]);

 private:
  OneWireSlave* slave_ = nullptr;
};
