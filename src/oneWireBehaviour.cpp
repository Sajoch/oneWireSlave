#include "oneWireBehaviour.h"

#include <stm32f0xx.h>

OneWireBehaviour::OneWireBehaviour(OneWireLowLevel *const lowLevel,
		TIM_HandleTypeDef *timer) :
		timer_(timer), lowLevel_(lowLevel) {
}

bool OneWireBehaviour::checkReset() {
	isResetInProgress_ = false;
	lowLevel_->setAsInput();
	if (!lowLevel_->read())
		return false;

	if (readWhile(true, OneWireTime::RESET_TIMEOUT))
		return false;

	auto resetTime = waitWhile(false, OneWireTime::RESET_MAX + 1);
	if (resetTime > OneWireTime::RESET_MIN
			&& resetTime < OneWireTime::RESET_MAX)
		return true;
	return false;
}

bool OneWireBehaviour::showPresence() {
	readWhile(true, OneWireTime::PRESENCE_TIMEOUT);
	lowLevel_->setAsOutput();
	lowLevel_->writeLow();
	wait(OneWireTime::PRESENCE_MIN);
	lowLevel_->setAsInput();
	auto maxWaitTime = OneWireTime::PRESENCE_MAX - OneWireTime::PRESENCE_MIN;
	auto full = waitWhile(false, maxWaitTime);
	if (full > maxWaitTime)
		return false;
	return true;
}

bool OneWireBehaviour::sendAddress(const uint8_t address[8]) {
	//__disable_irq();
	bool status = true;
	for (uint8_t byteId = 0; byteId < 8; byteId++) {
		if (!sendAddressByte(address[byteId])) {
			status = false;
			break;
		}
	}
	//__enable_irq();
	return status;
}

bool OneWireBehaviour::sendBit(bool bit) {
	auto waited = waitWhile(false, OneWireTime::SLOT_MAX);
	if (waited > OneWireTime::SLOT_MAX) {
		isResetInProgress_ = true;
		return false;
	}
	waited = waitWhile(true, OneWireTime::MSG_HIGH_TIMEOUT);
	if (waited > OneWireTime::MSG_HIGH_TIMEOUT)
		return false;

	//GPIOA->BSRR = 2;
	if (bit) {
		wait(OneWireTime::READ_MAX);
	} else {
		lowLevel_->setAsOutput();
		lowLevel_->writeLow();
		wait(OneWireTime::WRITE_ZERO);
	}

	// TODO: we should check for (!retries) because there
	// could be a reset in progress...
	lowLevel_->setAsInput();
	//GPIOA->BRR = 2;
	return true;
}

bool OneWireBehaviour::recvBit(bool &bit) {
	if (readWhile(false, OneWireTime::SLOT_MAX)) {
		isResetInProgress_ = true;
		return false;
	}

	if (readWhile(true, OneWireTime::MSG_HIGH_TIMEOUT))
		return false;

	//GPIOA->BSRR = 2;
	auto readValue = waitWhile(false, OneWireTime::READ_MIN);
	if (readValue < OneWireTime::READ_MIN) {
		bit = true;
	} else {
		bit = false;
	}
	//GPIOA->BRR = 2;
	return true;
}

bool OneWireBehaviour::readWhile(bool value, OneWireTime::timeOW_t time) {
	return waitWhile(value, time) > time;
}

uint32_t OneWireBehaviour::waitWhile(bool value, OneWireTime::timeOW_t time) {
	__HAL_TIM_SET_COUNTER(timer_, 0);
	while (lowLevel_->read() == value && __HAL_TIM_GET_COUNTER(timer_) < time)
		;
	return __HAL_TIM_GET_COUNTER(timer_);
}

void OneWireBehaviour::wait(OneWireTime::timeOW_t time) {
	// clear the us timer
	// replace (--time != 0) and (time == 0) by (timer_value > time)
	// TODO
	__HAL_TIM_SET_COUNTER(timer_, 0);
	while (__HAL_TIM_GET_COUNTER(timer_) < time)
		;
}

bool OneWireBehaviour::sendAddressByte(uint8_t byte) {
	bool checkBit;
	for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
		auto bit = static_cast<bool>(bitMask & byte);
		if (!sendBit(bit))
			return false;
		if (!sendBit(!bit))
			return false;
		if (!recvBit(checkBit))
			return false;
		if (checkBit != bit)
			return false;
	}
	return true;
}
