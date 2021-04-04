#pragma once

#include "oneWireBehaviour.h"
#include "oneWireChannel.h"
#include "oneWireController.h"
#include "iOneWireLowLevel.h"
#include "oneWireSlaveList.h"

class OneWireHub {
public:
	OneWireHub(IOneWireLowLevel &lowLevel);
	void poll();
	OneWireSlave* create(uint8_t address[8],
			std::shared_ptr<SlaveBehaviour> behaviour);

private:
	IOneWireLowLevel &lowLevel_;
	OneWireBehaviour behaviour_;
	OneWireChannel channel_;
	OneWireController controller_;
	OneWireSlaveList list_;
};
