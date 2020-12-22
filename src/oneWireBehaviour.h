#pragma once

#include "oneWireChannel.h"
#include "oneWireError.h"
#include "oneWireSlave.h"
#include "oneWireSlaveList.h"

class OneWireBehaviour {
 public:
  OneWireBehaviour(OneWireSlaveList* const slaveList,
                   OneWireChannel* const channel);
  bool processCommand();

 private:
  bool searchRomCommand();
  bool matchRomCommand();
  bool skipRomCommand();
  bool readRomCommand();
  bool alarmSearchCommand();
  bool resumeCommand();

  OneWireSlave* selectedSlave_;
  OneWireSlaveList* slaveList_;
  OneWireChannel* channel_;
  Error error_;
};