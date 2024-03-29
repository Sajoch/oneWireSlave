#pragma once

#include "iOneWireLowLevel.h"
#include "oneWireTime.h"
#include "stm32f0xx_hal.h"

class OneWireBehaviour {
 public:
  OneWireBehaviour(IOneWireLowLevel* const lowLevel);
  bool checkReset();
  bool showPresence();
  bool sendAddress(const uint8_t address[8]);
  bool sendBit(bool bit);
  bool recvBit(bool& bit);
  bool readWhile(bool value, OneWireTime::timeOW_t time);
  uint32_t waitWhile(bool value, OneWireTime::timeOW_t time);
  void wait(OneWireTime::timeOW_t time);
 private:
  bool sendAddressByte(const uint8_t byte);

  IOneWireLowLevel* lowLevel_;
  bool isResetInProgress_ = false;
};
