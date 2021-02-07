#include "oneWireLowLevel.h"

#include "oneWireTime.h"

OneWireLowLevel::OneWireLowLevel(GPIO_TypeDef *port, uint32_t pin) :
		port_(port), pin_(pin), pinModeMaskInput_(~pin_) {
}

bool OneWireLowLevel::read() {
	return port_->IDR & pin_;
}
void OneWireLowLevel::writeHigh() {
	port_->BSRR = pin_;
}
void OneWireLowLevel::writeLow() {
	port_->BRR = pin_;
}

void OneWireLowLevel::setAsOutput() {
	port_->MODER |= pin_;
}

void OneWireLowLevel::setAsInput() {
	port_->MODER &= pinModeMaskInput_;
}
