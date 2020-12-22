#include "oneWireBehaviour.h"

#include <cinttypes>

enum class OneWireCommandType : uint8_t {
  NONE = 0,
  SEARCH_ROM = 0xF0,
  MATCH_ROM_OVERDRIVE = 0x69,
  MATCH_ROM = 0x55,
  SKIP_ROM = 0x3C,
  SKIP_ROM_OVERDRIVE = 0xCC,
  READ_ROM_OLD = 0x0F,
  READ_ROM = 0x33,
  ALARM_SEARCH = 0xEC,
  RESUME_COMMAND = 0xA5
};

OneWireBehaviour::OneWireBehaviour(OneWireSlaveList* const slaveList,
                                   OneWireChannel* const channel)
    : slaveList_(slaveList), channel_(channel) {}

bool OneWireBehaviour::processCommand() {
  uint8_t cmd;
  channel_->recv(&cmd, 1);

  if (error_ == Error::RESET_IN_PROGRESS)
    return false;  // stay in poll()-loop and trigger another
                   // datastream-detection
  if (error_ != Error::NO_ERROR) return true;

  switch ((OneWireCommandType)cmd) {
    case OneWireCommandType::SEARCH_ROM:
      return searchRomCommand();
    case OneWireCommandType::MATCH_ROM_OVERDRIVE:
    case OneWireCommandType::MATCH_ROM:
      matchRomCommand();
      break;
    case OneWireCommandType::SKIP_ROM_OVERDRIVE:
    case OneWireCommandType::SKIP_ROM:
      skipRomCommand();
      break;
    case OneWireCommandType::READ_ROM_OLD:
    case OneWireCommandType::READ_ROM:
      return readRomCommand();
    case OneWireCommandType::ALARM_SEARCH:
      alarmSearchCommand();
      break;
    case OneWireCommandType::RESUME_COMMAND:
      if (!resumeCommand()) return false;
      break;
    default:
      error_ = Error::INCORRECT_ONEWIRE_CMD;
  }

  if (error_ == Error::RESET_IN_PROGRESS) return false;

  return (error_ != Error::NO_ERROR);
}

bool OneWireBehaviour::searchRomCommand() {
  selectedSlave_ = nullptr;
  // TODO searchIDTree();
  return false;
}
bool OneWireBehaviour::matchRomCommand() {
  uint8_t address[8];
  selectedSlave_ = nullptr;

  if (channel_->recv(address, 8)) {
    return true;
  }

  selectedSlave_ = slaveList_->find(address);

  if (selectedSlave_) selectedSlave_->duty(channel_);
  return true;
}
bool OneWireBehaviour::skipRomCommand() {
  // NOTE: If more than one slave is present on the bus,
  // and a read command is issued following the Skip ROM command,
  // data collision will occur on the bus as multiple slaves transmit
  // simultaneously
  if (selectedSlave_ == nullptr && slaveList_->isSingle()) {
    selectedSlave_ = slaveList_->getFirst();
  }
  if (selectedSlave_ != nullptr) {
    selectedSlave_->duty(channel_);
  }
  return true;
}
bool OneWireBehaviour::readRomCommand() {
  // only usable when there is ONE slave on the bus
  if (selectedSlave_ == nullptr && slaveList_->isSingle()) {
    selectedSlave_ = slaveList_->getFirst();
  }
  if (selectedSlave_ != nullptr) {
    selectedSlave_->sendID(channel_);
  }
  return false;
}
bool OneWireBehaviour::alarmSearchCommand() {
  // TODO: Alarm searchIDTree command, respond if flag is set
  // is like searchIDTree-rom, but only slaves with triggered alarm will
  // appear
  return true;
}
bool OneWireBehaviour::resumeCommand() {
  if (selectedSlave_ == nullptr) return false;
  if (selectedSlave_->duty(channel_)) {
    error_ = Error::INCORRECT_SLAVE_USAGE;
  }
  return true;
}