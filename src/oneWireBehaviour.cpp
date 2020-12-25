#include "oneWireBehaviour.h"

#include <stm32f0xx.h>

OneWireBehaviour::OneWireBehaviour(OneWireChannel* const channel,
                                   OneWireLowLevel* const lowLevel)
    : channel_(channel), lowLevel_(lowLevel) {}

bool OneWireBehaviour::checkReset() {
  isResetInProgress_ = false;
  lowLevel_->setAsInput();
  if (!lowLevel_->read()) return false;
  if (readWhile(true, OneWireTime::RESET_TIMEOUT)) return false;
  if (readWhile(false, OneWireTime::RESET_MIN - 1)) return false;
  if (readWhile(false, OneWireTime::RESET_MAX - OneWireTime::RESET_MIN + 1))
    return true;
  return false;
}

bool OneWireBehaviour::showPresence() {
  readWhile(true, OneWireTime::PRESENCE_TIMEOUT);
  lowLevel_->writeLow();
  lowLevel_->setAsOutput();
  wait(OneWireTime::PRESENCE_MIN);
  lowLevel_->setAsInput();
  if (readWhile(false, OneWireTime::PRESENCE_MAX - OneWireTime::PRESENCE_MIN)) {
    return false;
  }
  return true;
}

bool OneWireBehaviour::sendAddress(const uint8_t address[8]) {
  __disable_irq();
  for (uint8_t byteId = 0; byteId < 8; byteId++) {
    if (!sendAddressByte(address[byteId])) return false;
  }
  __enable_irq();
  return true;
}

bool OneWireBehaviour::sendBit(bool bit) {
  if (readWhile(false, OneWireTime::SLOT_MAX)) {
    isResetInProgress_ = true;
    return false;
  }

  if (readUntil(false, OneWireTime::MSG_HIGH_TIMEOUT)) return false;

  if (bit) {
    readWhile(false, OneWireTime::READ_MAX);
  } else {
    lowLevel_->setAsOutput();
    readWhile(false, OneWireTime::WRITE_ZERO);
  }

  // TODO: we should check for (!retries) because there
  // could be a reset in progress...
  lowLevel_->setAsInput();
  return true;
}

bool OneWireBehaviour::recvBit(bool& bit) {
  if (readWhile(false, OneWireTime::SLOT_MAX)) {
    isResetInProgress_ = true;
    return false;
  }

  if (readUntil(false, OneWireTime::MSG_HIGH_TIMEOUT)) return false;
  return !readWhile(false, OneWireTime::READ_MIN);
}

bool OneWireBehaviour::readWhile(bool value, OneWireTime::timeOW_t time) {
  // TODO clear the us timer
  // replace (--time != 0) and (time == 0) by (timer_value > time)
  while (lowLevel_->read() == value && --time != 0)
    ;
  return time == 0;
}
bool OneWireBehaviour::readUntil(bool value, OneWireTime::timeOW_t time) {
  // TODO clear the us timer
  // replace (--time != 0) and (time == 0) by (timer_value > time)
  while (lowLevel_->read() != value && --time != 0)
    ;
  return time == 0;
}
void OneWireBehaviour::wait(OneWireTime::timeOW_t time) {
  // clear the us timer
  // replace (--time != 0) and (time == 0) by (timer_value > time)
  // TODO
  while (--time != 0)
    ;
}

bool OneWireBehaviour::sendAddressByte(uint8_t byte) {
  bool checkBit;
  for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
    auto bit = static_cast<bool>(bitMask & byte);
    if (!sendBit(bit)) return false;
    if (!sendBit(!bit)) return false;
    if (!recvBit(checkBit)) return false;
    if (checkBit != bit) return false;
  }
  return true;
}