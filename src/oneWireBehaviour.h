#pragma once

#include "oneWireChannel.h"
#include "oneWireLowLevel.h"

class OneWireBehaviour {
 public:
  OneWireBehaviour(OneWireLowLevel* const lowLevel);
  bool checkReset();
  bool showPresence();
  bool sendAddress(const uint8_t address[8]);
  bool sendBit(bool bit);
  bool recvBit(bool& bit);
  bool readWhile(bool value, OneWireTime::timeOW_t time);
  bool readUntil(bool value, OneWireTime::timeOW_t time);
  void wait(OneWireTime::timeOW_t time);
 private:
  bool sendAddressByte(const uint8_t byte);

  OneWireChannel* channel_;
  OneWireLowLevel* lowLevel_;
  bool isResetInProgress_ = false;
};