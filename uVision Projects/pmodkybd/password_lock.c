// password_lock.c
#include "stm32f7xx.h"                  // Device header
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include "my_utils.h"
#include "my_kypd_utils.h"

extern GLCD_FONT GLCD_Font_16x24;

int main() {
	schar c, disp_update = ' ';
	char state = 0, lock = 0, lock_update;
	
	SystemClock_Config();
	GLCD_Initialize();
	
	// Initialize GPIOs
	RCC->AHB1ENR |= 1 << F | 1 << I;
	for (int i = 0; i < 4; i++)
		set_register_mode(I, i, 1);
	
	// Initialize screen LCD
	GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	
	GLCD_ClearScreen();
	
	GLCD_DrawString(50, 50, "[Password Lock]");
	
	while(1) {
		c = read_kypd();
		lock = (GPIOI->IDR & (1 << 11)) >> 11;
		if (disp_update != c || lock_update != lock) {
			disp_update = c;
			lock_update = lock;
			switch (state) {
				case 0:
					GLCD_DrawString(50, 75, "                   ");
					if (c == '9')
						state = 1;
					else
						state = 0;
					break;
				case 10:
					if (c == '4')
						state = 2;
					else
						state = 0;
					break;
				case 20:
					if (c == '8')
						state = 3;
					else
						state = 0;
					break;
				case 30:
					if (c == '7')
						state = 4;
					else
						state = 0;
					break;
				case 4:
					GLCD_DrawString(50, 75, "UNLOCKED           ");
					if (lock == 1)
						state = 0;
					break;
				case 1:
					if (c == -1)
						state = 10;
					break;
				case 2:
					if (c == -1)
						state = 20;
					break;
				case 3:
					if (c == -1)
						state = 30;
					break;
				
				default:
					GLCD_DrawString(50, 75, "ERR ERR ERR ERR ERR");
			}
		}
	}
}
