#pragma once
#include <cinttypes>

#include "oneWireBehaviour.h"
#include "iOneWireLowLevel.h"

class OneWireChannel {
 public:
  OneWireChannel(OneWireBehaviour* behaviour, IOneWireLowLevel* lowLevel);
  bool send(const uint8_t* data, int length);
  bool send(const uint8_t data);
  bool recv(uint8_t* data, int length);
  bool sendAddress(const uint8_t address[8]);

 private:
  bool sendByte(const uint8_t byte);
  bool recvByte(uint8_t& byte);
  OneWireBehaviour* behaviour_;
  IOneWireLowLevel* lowLevel_;
};
