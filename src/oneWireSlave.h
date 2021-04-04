#pragma once
#include <cinttypes>
#include <memory>

#include "oneWireChannel.h"
#include "slaveBehaviour.h"

class OneWireSlave {
 public:
  OneWireSlave(uint8_t id[8], std::shared_ptr<SlaveBehaviour> behaviour);
  void sendID(OneWireChannel* const channel) const;
  void replayForIDSearch(OneWireChannel* const channel) const;
  bool isMatchingId(uint8_t id[8]) const;
  bool duty(OneWireChannel* const channel) const;

 private:
  uint8_t ID[8];
  std::shared_ptr<SlaveBehaviour> behaviour_;
};
