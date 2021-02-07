#include "oneWireHub.h"

OneWireHub::OneWireHub(OneWireLowLevel &lowLevel, TIM_HandleTypeDef *timer) :
		lowLevel_(lowLevel), behaviour_(&lowLevel_, timer), channel_(
				&behaviour_, &lowLevel_), controller_(&list_, &channel_) {
}

void OneWireHub::poll() {
	if (!behaviour_.checkReset())
		return;
	if (!behaviour_.showPresence())
		return;
	if (!controller_.processCommand())
		return;
}

OneWireSlave* OneWireHub::create(uint8_t address[8],
		std::shared_ptr<SlaveBehaviour> behaviour) {
	return list_.create(address, behaviour);
}
