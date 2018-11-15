//#include "RTE_Device.h"                 // Keil::Device:STM32Cube Framework:Classic
//#include "RTE_Components.h"             // Component selection
#include "stm32f7xx.h"                  // Device header

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define K 0xa

typedef unsigned int uint;

void activate_peripheral(uint gpio) {
	RCC->AHB1ENR |= 1 << gpio;
}

/**
 * gpio name
 * pin  pin of gpio
 * mode 0 for read (input), 1 for write (ouput), 2 for alt function, 3 for analog
 */
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

int main() {
	// initialization
	// activate peripheral
	RCC->AHB1ENR |= 1 << A | 1 << C | 1 << F | 1 << G | 1 << I;
	
	// set pin mode
	// set switches to input
	for (int i = 10; i > 5; i--)
		set_register_mode(F, i, 0);
	set_register_mode(A, 15, 0);
	set_register_mode(A, 8, 0);
	set_register_mode(A, 0, 0);
	// set leds to output
	set_register_mode(G, 7, 1);
	set_register_mode(G, 6, 1);
	set_register_mode(C, 7, 1);
	set_register_mode(C, 6, 1);
	for (int i = 3; i > -1; i--)
		set_register_mode(I, i, 1);
	
	uint portA, portF, INPUT = 0, OUTPUT = 0;
	uint TAB[1<<0x8] = {0};
	
	// generate truth table for equation set 2 
	for (int INPUT = 0; INPUT < 1<<0x8; INPUT++) {
		if ((INPUT & 0xF) == 0xF || (INPUT & 0x60) == 0x60 || (INPUT & 0x80) == 0x00)
			TAB[INPUT] |= 0x1;
		if ((INPUT & 0xC1) == 0x41 || (INPUT & 0x70) == 0x50)
			TAB[INPUT] |= 0x2;
		if ((INPUT & 0x17) == 0x17 || (INPUT & 0xc0) == 0x00) 
			TAB[INPUT] |= 0x4;
		if ((INPUT & 0x54) == 0x44 || (INPUT & 0x81) == 0x01 || (INPUT & 0x30) == 0x30)
			TAB[INPUT] |= 0x8;
		if ((INPUT & 0xA) == 0xA || (INPUT & 0x60) == 0x60)
			TAB[INPUT] |= 0x10;
		if ((INPUT & 0x3) == 0x3)
			TAB[INPUT] |= 0x20;
	}
	
	while (1) {
		// get current state
		
		
//		INPUT = GPIOA->IDR & 0b00000001;
		
		portA = GPIOA->IDR;
		portF = GPIOF->IDR;
//		INPUT = 0;
		// create and combine inputs
		INPUT = (portA&(1<<0)) | (portA&(1<<8)) >> 7 | (portA&(1<<15)) >> 13 | (portF&0x7c0) >> 3;

		
		// process input
		
		/*if (INPUT == 0b00000001)
			OUTPUT |= 0b00000001;
		*/
		
		/*if ((INPUT & 0b00000011) == 0b01 || (INPUT & 0b00111000) == 0b011 << 3)
			OUTPUT |= 0b00000001;
		if ((INPUT & 0b10000000) == 0b1 << 7)
			OUTPUT |= 0b00000010;
		if ((INPUT & 0b01000000) == 0b0 << 6)
			OUTPUT |= 0b00000100;
		if ((INPUT & 0b00001100) == 0b11 << 2)
			OUTPUT |= 0b00001000;
		*/

		/*if ((INPUT & 0xF) == 0xF || (INPUT & 0x60) == 0x60 || (INPUT & 0x80) == 0x00)
			OUTPUT |= 0x1;
		if ((INPUT & 0xC1) == 0x41 || (INPUT & 0x70) == 0x50)
			OUTPUT |= 0x2;
		if ((INPUT & 0x17) == 0x17 || (INPUT & 0xc0) == 0x00) 
			OUTPUT |= 0x4;
		if ((INPUT & 0x54) == 0x44 || (INPUT & 0x81) == 0x01 || (INPUT & 0x30) == 0x30)
			OUTPUT |= 0x8;
		if ((INPUT & 0xA) == 0xA || (INPUT & 0x60) == 0x60)
			OUTPUT |= 0x10;
		if ((INPUT & 0x3) == 0x3)
			OUTPUT |= 0x20;*/
		
		OUTPUT = TAB[INPUT];
		
		
		// output
		
		/*GPIOG->ODR = (GPIOG->ODR & 0b11000000) | OUTPUT;
		GPIOC->ODR = (GPIOC->ODR & 0b11000000) | OUTPUT << 2;
		GPIOI->ODR = (GPIOI->ODR & 0b00001111) | OUTPUT;
		*/
		
		
		GPIOG->BSRR |= (OUTPUT & 0b11000000) | 0xFFFF0000;
		GPIOC->BSRR |= (OUTPUT & 0b00110000) << 2 | 0xFFFF0000;
		GPIOI->BSRR |= (OUTPUT & 0b00001111) | 0xFFFF0000;
		
		//activate_led(8, 0);
	}
}
