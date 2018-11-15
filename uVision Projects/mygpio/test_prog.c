// Test program
#include "stm32f7xx.h"                  // Device header
#include "my_utils.h"

int main() {
	// initialization
	// activate peripheral
	activate_peripheral(A);
	activate_peripheral(I);
	
	// set pin mode
	// set switches to input
	set_register_mode(A, 0, 0);
	// set leds to output
	set_register_mode(I, 0, 1);
	set_register_mode(I, 1, 1);
	set_register_mode(I, 2, 1);
	
	uint INPUT, OUTPUT;
	
	activate_led(I, 3);
	
	while (1) {
		OUTPUT = 0;
		// get current state
		INPUT = GPIOA->IDR & 0b00000001;

		// process input
		if (INPUT == 0b00000001)
			OUTPUT |= 0b00000111;
		
		// output
		GPIOI->ODR = (GPIOI->ODR & ~0b00000111) | (OUTPUT & 0b00000111);
	}
}

