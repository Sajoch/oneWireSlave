#include "ds18b20.h"

#include "../oneWireCRC.h"

DS18B20::DS18B20() {
  scratchpad_[0] = 0xA0;  // TLSB --> 10 degC as std
  scratchpad_[1] = 0x00;  // TMSB
  scratchpad_[2] = 0x4B;  // THRE --> Trigger register TH
  scratchpad_[3] = 0x46;  // TLRE --> TLow
  scratchpad_[4] = 0x7F;  // Conf
  // = 0 R1 R0 1 1 1 1 1 --> R=0 9bit .... R=3 12bit
  scratchpad_[5] = 0xFF;  // 0xFF
  scratchpad_[6] = 0x00;  // Reset
  scratchpad_[7] = 0x10;  // 0x10
  scratchpad_[8] = OneWireCRC::crc8(scratchpad_, 8);
}

bool DS18B20::duty(OneWireChannel* const channel) {
  uint8_t cmd;
  if (channel->recv(&cmd, 1)) return false;

  switch (cmd) {
    case 0x4E:  // WRITE SCRATCHPAD
      // write 3 byte of data to scratchpad[2:4], ds18s20 only first 2 bytes
      // (TH, TL)
      channel->recv(&scratchpad_[2], 3);  // dont return here, so crc gets
                                          // updated even if write not complete
      scratchpad_[8] = OneWireCRC::crc8(scratchpad_, 8);
      break;

    case 0xBE:  // READ SCRATCHPAD
      channel->send(scratchpad_, 9);
      break;
    case 0xB8:  // RECALL E2 (3 byte EEPROM to Scratchpad[2:4])
      break;    // signal that OP is done, 1s is passive ...

    case 0xB4:  // READ POWER SUPPLY
      break;
    case 0x44:  // CONVERT T --> start a new measurement conversion
      // we have 94 ... 750ms time here (9-12bit conversion)
      break;  // send 1s, is passive ...

    default:
      return false;
  }
  return true;
}
