#include "oneWireHub.h"

OneWireHub::OneWireHub(OneWireLowLevel& lowLevel)
    : lowLevel_(lowLevel),
      behaviour_(&lowLevel_),
      channel_(&lowLevel_),
      controller_(nullptr, &channel_) {}

void OneWireHub::poll() {
  while (true) {
    if (!list_.isAny()) return;
    if (!behaviour_.checkReset()) return;
    if (!behaviour_.showPresence()) return;
    if (!controller_.processCommand()) return;
  }
}

OneWireSlave* OneWireHub::create(uint8_t address[8]) {
  return list_.create(address);
}