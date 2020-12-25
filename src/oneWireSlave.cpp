#include "oneWireSlave.h"

void OneWireSlave::sendID(OneWireChannel* const channel) const {
  channel->send(ID, 8);
}

void OneWireSlave::replayForIDSearch(OneWireChannel* const channel) const {
  channel->sendAddress(ID);
}

bool OneWireSlave::duty(OneWireChannel* const channel) const {
  if (behaviour) return behaviour->duty(channel);
  return false;
}