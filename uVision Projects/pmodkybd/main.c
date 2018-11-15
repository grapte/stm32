#include "stm32f7xx.h"                  // Device header
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include "my_utils.h"
#include "my_kypd_utils.h"

extern GLCD_FONT GLCD_Font_16x24;

int main() {
	schar c[20] = "                   ";
	schar prev = ' ';
	
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
	
	GLCD_DrawString(50, 50, "You've Pressed:");
	
	while(1) {
		c[0] = read_kypd();
		if (c[0] != prev) {
			prev = c[0];
			switch (c[0]) {
				case -1:
					GLCD_DrawString(50, 75, "No keys.           ");
					break;
				case -2:
					GLCD_DrawString(50, 75, "More than one key. ");
					break;
				default:
					GLCD_DrawString(50, 75, (const char *) c);
			}
		}
	}
}
