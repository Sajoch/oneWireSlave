#pragma once

#include "oneWireBehaviour.h"
#include "oneWireChannel.h"
#include "oneWireController.h"
#include "oneWireLowLevel.h"
#include "oneWireSlaveList.h"

class OneWireHub {
 public:
  OneWireHub(OneWireLowLevel& lowLevel);
  void poll();
  OneWireSlave* create(uint8_t address[8]);

 private:
  OneWireLowLevel& lowLevel_;
  OneWireChannel channel_;
  OneWireBehaviour behaviour_;
  OneWireController controller_;
  OneWireSlaveList list_;
};