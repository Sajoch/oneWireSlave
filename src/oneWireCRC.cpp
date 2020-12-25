#include "oneWireCRC.h"

uint8_t OneWireCRC::crc8(const uint8_t* data, uint8_t size, uint8_t crcInit) {
  auto crc = crcInit;

  for (uint8_t index = 0; index < size; ++index) {
    auto inByte = data[index];
    for (uint8_t bitPosition = 0; bitPosition < 8; ++bitPosition) {
      const uint8_t mix = (crc ^ inByte) & static_cast<uint8_t>(0x01);
      crc >>= 1;
      if (mix != 0) crc ^= 0x8C;
      inByte >>= 1;
    }
  }
  return crc;
}
uint16_t OneWireCRC::crc16(const uint8_t address[], uint8_t len,
                           uint16_t init) {
  return 0;
}
uint16_t OneWireCRC::crc16(uint8_t value, uint16_t crc) { return 0; }