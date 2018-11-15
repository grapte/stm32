// Combinational System 1
#include "stm32f7xx.h"                  // Device header
#include "my_utils.h"

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
	for (int i = 3; i > -1; i--)
		set_register_mode(I, i, 1);
	
	uint portA, portF, INPUT, OUTPUT;
	
	while (1) {
		OUTPUT = 0;
		// get current state
		portA = GPIOA->IDR;
		portF = GPIOF->IDR;
		// create and combine inputs
		INPUT = (portA&(1<<0)) | (portA&(1<<8)) >> 7 | (portA&(1<<15)) >> 13 | (portF&0x7c0) >> 3;

		// process input
		if ((INPUT & 0b00000011) == 0b01 || (INPUT & 0b00111000) == 0b011 << 3)
			OUTPUT |= 0b00000001;
		if ((INPUT & 0b10000000) == 0b1 << 7)
			OUTPUT |= 0b00000010;
		if ((INPUT & 0b01000000) == 0b0 << 6)
			OUTPUT |= 0b00000100;
		if ((INPUT & 0b00001100) == 0b11 << 2)
			OUTPUT |= 0b00001000;
		
		// output
//		GPIOG->ODR = (GPIOG->ODR & ~0b11000000) | (OUTPUT & 0b11000000);
		GPIOC->ODR = (GPIOC->ODR & ~0b11000000) | (OUTPUT << 2 & 0b11000000);
		GPIOI->ODR = (GPIOI->ODR & ~0b00001111) | (OUTPUT & 0b00001111);
		
	}
}
