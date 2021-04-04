#pragma once
#include <cinttypes>

#include "oneWireBehaviour.h"
#include "oneWireLowLevel.h"

class OneWireChannel {
 public:
  OneWireChannel(OneWireBehaviour* behaviour, IOneWireLowLevel* lowLevel);
  bool send(const uint8_t* data, int length);
  bool recv(uint8_t* data, int length);
  bool sendAddress(const uint8_t address[8]);

 private:
  bool sendByte(const uint8_t byte);
  bool recvByte(uint8_t& byte);
  OneWireBehaviour* behaviour_;
  IOneWireLowLevel* lowLevel_;
};
