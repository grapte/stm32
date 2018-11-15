// my_kypd_utils.c
#include "stm32f7xx.h"                  // Device header
#include "my_kypd_utils.h"

schar hexaKeys[4][4] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'0','F','E','D'}
};

schar read_kypd() {
	char dupe = 0;
	schar out = -1;
	
	GPIOI->BSRR = 0xf;
	
	// Read every column
	for (char i = 0; i < 4; i++) {
		GPIOI->BSRR |= 1 << (0x10 + i);
		
		// loop over rows
		for (char j = 6; j < 10; j++) {
			// read input of key of row and column
			if (~GPIOF->IDR & 1 << j) {
				if (dupe)
					return -2;
				dupe = 1;
				out = hexaKeys[j-6][i];
			}
		}
		GPIOI->BSRR |= 1 << i;
	}
	
	return out;
}
