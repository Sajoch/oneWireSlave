#pragma once
#include <cinttypes>

#include "oneWireBehaviour.h"
#include "oneWireLowLevel.h"

class OneWireChannel {
 public:
  OneWireChannel(OneWireBehaviour* behaviour, OneWireLowLevel* lowLevel);
  bool send(const uint8_t* data, uint8_t length);
  bool recv(uint8_t* data, uint8_t length);
  bool sendAddress(const uint8_t address[8]);

 private:
  bool sendByte(const uint8_t byte);
  bool recvByte(uint8_t& byte);
  OneWireBehaviour* behaviour_;
  OneWireLowLevel* lowLevel_;
};
