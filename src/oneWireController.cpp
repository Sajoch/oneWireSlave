#include "oneWireController.h"

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

OneWireController::OneWireController(OneWireSlaveList* const slaveList,
                                     OneWireChannel* const channel)
    : slaveList_(slaveList), channel_(channel) {}

bool OneWireController::processCommand() {
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

bool OneWireController::searchRomCommand() {
  selectedSlave_ = nullptr;
  auto slave = slaveList_->getFirst();
  if (slave) {
    slave->replayForIDSearch(channel_);
    selectedSlave_ = slave;
  }
  return false;
}
bool OneWireController::matchRomCommand() {
  uint8_t address[8];
  selectedSlave_ = nullptr;

  if (channel_->recv(address, 8)) return true;

  selectedSlave_ = slaveList_->find(address);

  if (selectedSlave_) selectedSlave_->duty(channel_);
  return true;
}
bool OneWireController::skipRomCommand() {
  if (selectedSlave_ == nullptr && slaveList_->isSingle()) {
    selectedSlave_ = slaveList_->getFirst();
  }
  if (selectedSlave_ != nullptr) selectedSlave_->duty(channel_);
  return true;
}
bool OneWireController::readRomCommand() {
  if (selectedSlave_ == nullptr && slaveList_->isSingle()) {
    selectedSlave_ = slaveList_->getFirst();
  }
  if (selectedSlave_ != nullptr) selectedSlave_->sendID(channel_);
  return false;
}
bool OneWireController::alarmSearchCommand() {
  // TODO: Alarm searchIDTree command, respond if flag is set
  // is like searchIDTree-rom, but only slaves with triggered alarm will
  // appear
  return true;
}
bool OneWireController::resumeCommand() {
  if (selectedSlave_ == nullptr) return false;
  if (selectedSlave_->duty(channel_)) {
    error_ = Error::INCORRECT_SLAVE_USAGE;
  }
  return true;
}