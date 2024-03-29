#include "oneWireChannel.h"

#include <stm32f0xx.h>

OneWireChannel::OneWireChannel(OneWireBehaviour *behaviour,
		IOneWireLowLevel *lowLevel) :
		behaviour_(behaviour), lowLevel_(lowLevel) {
}

bool OneWireChannel::send(const uint8_t *data, int length) {
	__disable_irq();
	lowLevel_->setAsInput();
	lowLevel_->writeLow();
	int sent = 0;

	for (; sent < length; ++sent) {
		const uint8_t dataByte = data[sent];
		if (!sendByte(dataByte))
			break;
	}
	__enable_irq();
	return (sent != length);
}

bool OneWireChannel::send(const uint8_t data) {
	__disable_irq();
	lowLevel_->setAsInput();
	lowLevel_->writeLow();

	if (!sendByte(data))
		return false;

	__enable_irq();
	return true;
}

bool OneWireChannel::recv(uint8_t *data, int length) {
	__disable_irq();
	lowLevel_->setAsInput();

	int received = 0;
	for (; received < length; ++received) {
		if (!recvByte(data[received]))
			break;
	}
	__enable_irq();
	return (received != length);
}

bool OneWireChannel::sendAddress(const uint8_t address[8]) {
	return behaviour_->sendAddress(address);
}

bool OneWireChannel::sendByte(const uint8_t byte) {
	for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
		auto bit = static_cast<bool>(bitMask & byte);
		if (!behaviour_->sendBit(bit))
			return false;
	}
	return true;
}

bool OneWireChannel::recvByte(uint8_t &byte) {
	bool bit;
	byte = 0;
	for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
		if (!behaviour_->recvBit(bit))
			return false;
		if (bit)
			byte |= bitMask;
	}
	return true;
}
