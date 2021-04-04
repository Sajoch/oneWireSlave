#pragma once

#include "oneWireBehaviour.h"
#include "oneWireChannel.h"
#include "oneWireController.h"
#include "oneWireLowLevel.h"
#include "oneWireSlaveList.h"
#include "stm32f0xx_hal.h"

class OneWireHub {
 public:
  OneWireHub(IOneWireLowLevel& lowLevel, TIM_HandleTypeDef* timer);
  void poll();
  OneWireSlave* create(uint8_t address[8], std::shared_ptr<SlaveBehaviour> behaviour);

 private:
  IOneWireLowLevel& lowLevel_;
  OneWireBehaviour behaviour_;
  OneWireChannel channel_;
  OneWireController controller_;
  OneWireSlaveList list_;
};
