// Combinational System 2
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
	set_register_mode(G, 7, 1);
	set_register_mode(G, 6, 1);
	set_register_mode(C, 7, 1);
	set_register_mode(C, 6, 1);
	for (int i = 3; i > -1; i--)
		set_register_mode(I, i, 1);
	
	uint portA, portF, INPUT, OUTPUT;
	uint count = 1000000;
	
	while (1) {
		OUTPUT = 0;
	  // get current state
		portA = GPIOA->IDR;
		portF = GPIOF->IDR;
		// create and combine inputs
		INPUT = (portA&(1<<0)) | (portA&(1<<8)) >> 7 | (portA&(1<<15)) >> 13 | (portF&0x7c0) >> 3;

		// process input
		if ((INPUT & 0xF) == 0xF || (INPUT & 0x60) == 0x60 || (INPUT & 0x80) == 0x00)
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
			OUTPUT |= 0x20;
		
		// output
//		GPIOG->BSRR |= (OUTPUT & 0b11000000) | 0xFFFF0000;
		GPIOC->BSRR |= (OUTPUT & 0b00110000) << 2 | 0xFFFF0000;
		GPIOI->BSRR |= (OUTPUT & 0b00001111) | 0xFFFF0000;
		count++;
		if (count > 1000000)
   			count = 0;
	}
}
