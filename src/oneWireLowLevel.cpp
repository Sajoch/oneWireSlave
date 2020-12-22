#include "oneWireLowLevel.h"

#include "oneWireTime.h"

bool OneWireLowLevel::sendBit(bool bit) {
  // Wait for bus to rise HIGH, signaling end of last timeslot

  if (readWhile(false, OneWireTime::SLOT_MAX)) {
    _error = Error::RESET_IN_PROGRESS;
    return true;
  }

  // Wait for bus to fall LOW, start of new timeslot
  if (readUntil(false, OneWireTime::MSG_HIGH_TIMEOUT)) {
    _error = Error::AWAIT_TIMESLOT_TIMEOUT_HIGH;
    return true;
  }

  // first difference to inner-loop of read()
  if (bit) {
    readWhile(0, OneWireTime::READ_MAX);
  } else {
    setAsOutput();
    readWhile(0, OneWireTime::WRITE_ZERO);
  }

  // TODO: we should check for (!retries) because there
  // could be a reset in progress...
  setAsInput();
  return false;
}

bool OneWireLowLevel::recvBit(bool& bit) {
  if (readWhile(false, OneWireTime::SLOT_MAX)) {
    _error = Error::RESET_IN_PROGRESS;
    return false;
  }

  // Wait for bus to fall LOW, start of new timeslot
  if (readUntil(false, OneWireTime::MSG_HIGH_TIMEOUT)) {
    _error = Error::AWAIT_TIMESLOT_TIMEOUT_HIGH;
    return false;
  }

  // wait a specific time to do a read (data is valid by then), // first
  // difference to inner-loop of write()
  return !readWhile(false, OneWireTime::READ_MIN);
}

bool OneWireLowLevel::readWhile(bool value, OneWireTime::timeOW_t time) {
  while (read() == value && --time != 0)
    ;
  return time == 0;
}
bool OneWireLowLevel::readUntil(bool value, OneWireTime::timeOW_t time) {
  while (read() != value && --time != 0)
    ;
  return time == 0;
}

bool OneWireLowLevel::read() {
  return false;  // TODO
}
void OneWireLowLevel::write(bool value) {
  // TODO
}

void OneWireLowLevel::setAsOutput() {}
void OneWireLowLevel::setAsInput() {}