#pragma once
#include <cinttypes>

class OneWireTime {
 public:
  using timeOW_t = uint32_t;
  // for not hanging to long in reset-detection, lower value is
  // better for more responsive applications, but can miss resets
  static constexpr timeOW_t RESET_TIMEOUT = {5000};
  static constexpr timeOW_t RESET_MIN = {430};  // should be 480
  static constexpr timeOW_t RESET_MAX = {960};  // from ds2413

  // Presence: slave waits TIMEOUT and emits a low state after the reset with
  // ~MIN length, if the bus stays low after that and exceeds MAX the hub will
  // issue an error
  // probe measures 25us, duration of high state between reset and
  // presence
  static constexpr timeOW_t PRESENCE_TIMEOUT = {20};
  static constexpr timeOW_t PRESENCE_MIN = {160};  // was 125
  static constexpr timeOW_t PRESENCE_MAX = {480};  // should be 280, was 480

  // there can be these inactive / high timeperiods after reset /
  // presence, this value defines the timeout for these
  static constexpr timeOW_t MSG_HIGH_TIMEOUT = {15000};
  static constexpr timeOW_t SLOT_MAX = {
      135};  // should be 120, measured from falling edge to next falling edge

  // read and write from the viewpoint of the slave!!!!
  static constexpr timeOW_t READ_MIN = {
      20};  // should be 15, was 30, says when it is safe to read a valid bit

  // low states (zeros) of a master should
  // not exceed this time in a slot
  static constexpr timeOW_t READ_MAX = {60};
  static constexpr timeOW_t WRITE_ZERO = {30};
};
