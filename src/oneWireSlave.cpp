#include "oneWireSlave.h"
#include "oneWireCRC.h"
#include <string.h>

OneWireSlave::OneWireSlave(uint8_t id[8],
		std::shared_ptr<SlaveBehaviour> behaviour) :
		behaviour_(behaviour) {
	memcpy(ID, id, 7);
	ID[7] = OneWireCRC::crc8(id, 7);
}

void OneWireSlave::sendID(OneWireChannel *const channel) const {
	channel->send(ID, 8);
}

void OneWireSlave::replayForIDSearch(OneWireChannel *const channel) const {
	channel->sendAddress(ID);
}

bool OneWireSlave::isMatchingId(uint8_t id[8]) const {
	return memcmp(ID, id, 8) == 0;
}

bool OneWireSlave::duty(OneWireChannel *const channel) const {
	if (behaviour_)
		return behaviour_->duty(channel);
	return false;
}
