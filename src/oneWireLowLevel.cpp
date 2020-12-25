#include "oneWireLowLevel.h"

#include <stm32f0xx.h>

#include "oneWireTime.h"

OneWireLowLevel::OneWireLowLevel(uint32_t pin)
    : pin_(1 << pin),
      pinModeMaskOutput_(1 << (2 * pin)),
      pinModeMaskInput_(~pinModeMaskOutput_) {
  RCC->AHBENR |= (1 << 17);
}

bool OneWireLowLevel::read() { return GPIOA->IDR & pin_; }
void OneWireLowLevel::writeHigh() { GPIOA->BSRR = pin_; }
void OneWireLowLevel::writeLow() { GPIOA->BRR = pin_; }

void OneWireLowLevel::setAsOutput() { GPIOA->MODER |= pinModeMaskOutput_; }
void OneWireLowLevel::setAsInput() { GPIOA->MODER &= pinModeMaskInput_; }