#pragma once
#include "slaveBehaviour.h"

class DS18B20 : public SlaveBehaviour {
 public:
  DS18B20();
  bool duty(OneWireChannel* const channel) override;

 private:
  uint8_t scratchpad_[9];
};