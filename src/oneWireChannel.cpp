#include "oneWireChannel.h"

bool OneWireChannel::send(const uint8_t* data, uint8_t length) {
  // TODO noInterrupts();  // will be enabled at the end of function
  ll_->write(false);
  ll_->setAsInput();
  uint8_t sent = 0;

  for (; sent < length; ++sent) {
    const uint8_t dataByte = data[sent];
    if (!sendByte(dataByte)) break;
  }
  // TODO interrupts();
  return (sent != length);
}

bool OneWireChannel::recv(uint8_t* data, uint8_t length) {
  // TODO noInterrupts();  // will be enabled at the end of function
  ll_->write(false);
  ll_->setAsInput();

  uint8_t received = 0;
  for (; received < length; ++received) {
    if (!recvByte(data[received])) break;
  }

  // TODO interrupts();
  return (received != length);
}

bool OneWireChannel::sendByte(const uint8_t byte) {
  for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
    auto bit = static_cast<bool>(bitMask & byte);
    if (!ll_->sendBit(bit)) {
      if (bitMask == 0x01 && error_ == Error::AWAIT_TIMESLOT_TIMEOUT_HIGH)
        error_ = Error::FIRST_BIT_OF_BYTE_TIMEOUT;
      return false;
    }
  }
  return true;
}

bool OneWireChannel::recvByte(uint8_t& byte) {
  bool bit;
  byte = 0;
  for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
    if (!ll_->recvBit(bit)) {
      if (bitMask == 0x01 && error_ == Error::AWAIT_TIMESLOT_TIMEOUT_HIGH)
        error_ = Error::FIRST_BIT_OF_BYTE_TIMEOUT;
      return false;
    }
    if (bit) byte |= bitMask;
  }
  return true;
}