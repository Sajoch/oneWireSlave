#pragma once
#include <cinttypes>

#include "oneWireLowLevel.h"

class OneWireChannel {
 public:
  OneWireChannel(OneWireLowLevel* ll);
  bool send(const uint8_t* data, uint8_t length);
  bool recv(uint8_t* data, uint8_t length);

 private:
  bool sendByte(const uint8_t byte);
  bool recvByte(uint8_t& byte);
  OneWireLowLevel* ll_;
};