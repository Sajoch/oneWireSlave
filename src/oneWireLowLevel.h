#pragma once
#include "oneWireTime.h"
#include <stm32f0xx.h>

class OneWireLowLevel {
 public:
 OneWireLowLevel(GPIO_TypeDef* port, uint32_t pin);
  bool read();
  void writeHigh();
  void writeLow();
  void setAsOutput();
  void setAsInput();
  private:
  GPIO_TypeDef* port_;
  const uint32_t pin_;
  const uint32_t pinModeMaskInput_;
};
