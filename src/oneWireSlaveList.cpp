#include "oneWireSlaveList.h"

OneWireSlave* OneWireSlaveList::create(uint8_t address[8],
		std::shared_ptr<SlaveBehaviour> behaviour) {
	slave_ = new OneWireSlave(address, behaviour);
	return slave_;
}

OneWireSlave* OneWireSlaveList::getFirst() const {
	return slave_;
}

bool OneWireSlaveList::isSingle() const {
	return true;
}

bool OneWireSlaveList::isAny() const {
	return slave_ != nullptr;
}

OneWireSlave* OneWireSlaveList::find(uint8_t address[8]) const {

}
