#pragma once
#include <cinttypes>

class OneWireCRC {
 public:
  static uint8_t crc8(const uint8_t data[], uint8_t data_size,
                      uint8_t crc_init = 0);
  static uint16_t crc16(const uint8_t address[], uint8_t len,
                        uint16_t init = 0);
  static uint16_t crc16(uint8_t value, uint16_t crc);
};