#pragma once
#include "oneWireSlave.h"

class OneWireSlaveList {
public:
	OneWireSlave* create(uint8_t address[8],
			std::shared_ptr<SlaveBehaviour> behaviour);
	OneWireSlave* getFirst() const;
	bool isSingle() const;
	bool isAny() const;
	OneWireSlave* find(uint8_t address[8]) const;

private:
	OneWireSlave *slave_ = nullptr;
};
