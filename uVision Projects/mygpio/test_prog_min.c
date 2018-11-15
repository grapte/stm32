// Test program
#include "stm32f7xx.h"                  // Device header

int main() {
	// initialization
	// activate peripheral
	RCC->AHB1ENR |= 1 << 0 | 1 << 8; // ports A and I
	
	// set pin mode
	// set switches to input
	// pin PA0 is already on input by default
	// set leds to output
	GPIOI->MODER |= 1 << 2 * 2 | 1 << 1 * 2 | 1 << 0 * 2; // output mode for PI2, PI1, PI0
	
	unsigned int INPUT, OUTPUT;
	
	while (1) {
		// get current state
		INPUT = GPIOA->IDR & 0b00000001;

		// process input
		if (INPUT == 0b00000001)
			OUTPUT |= 0b00000111;
		else
			OUTPUT &= ~0b00000111;
		
		// output
		if ((OUTPUT & 0b00000111) == 0b00000111)
			GPIOI->ODR |= OUTPUT & 0b00000111;
		else
			GPIOI->ODR &= OUTPUT & 0b00000111;
	}
}
