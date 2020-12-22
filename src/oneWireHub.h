#pragma once

#include "oneWireChannel.h"
#include "oneWireLowLevel.h"
#include "oneWireSlaveList.h"

class OneWireHub {
 public:
  void poll();

 private:
  OneWireLowLevel& ll_;
};