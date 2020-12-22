#pragma once
#include "oneWireChannel.h"

class SlaveBehaviour {
 public:
  virtual bool duty(OneWireChannel* const channel) = 0;
};