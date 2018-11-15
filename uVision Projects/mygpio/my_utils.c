// my_utils.c
#include "stm32f7xx.h"                  // Device header
#include "my_utils.h"

void activate_peripheral(uint gpio) {
	RCC->AHB1ENR |= 1 << gpio;
}

void set_register_mode(uint gpio, uint pin, uint mode) {
	GPIO_TypeDef  *GPIOx;
	GPIOx = (GPIO_TypeDef *) (GPIOA_BASE + 0x400 * gpio);
	if ((gpio == A && (pin == 15 || pin == 14 || pin == 13)) || 
		  (gpio == B && (pin == 4 || pin == 3))) {
		GPIOx->MODER &= ~(0x3 << pin * 2);
	}
			
	GPIOx->MODER |= mode << pin * 2;
}

void activate_led(uint gpio, uint pin) {
	GPIO_TypeDef  *GPIOx;
	GPIOx = (GPIO_TypeDef *) (GPIOA_BASE + 0x400 * gpio);
	GPIOx->MODER |= 1 << pin * 2;
	GPIOx->ODR   |= 1 << pin;
}