#include "stm32f7xx.h"                  // Device header
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
//#include <stdlib.h>
#include "my_utils.h"
#include "my_kypd_utils.h"

static void SystemClock_Config(void);

extern GLCD_FONT GLCD_Font_16x24;

int main() {
	schar c, disp_update = ' ';
	char state = 0;
	
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
		if (disp_update != c) {
			disp_update = c;
			switch (state) {
				case 0:
					GLCD_DrawString(50, 75, "                   ");
					if (c == '9')
						state = 1;
					break;
				case 10:
					if (c == '4')
						state = 2;
					break;
				case 20:
					if (c == '8')
						state = 3;
					break;
				case 30:
					GLCD_DrawString(50, 75, "UNLOCKED           ");
					if (c == '7')
						state = 4;
					break;
				case 4:
					if (c != -1)
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

void SysTick_Handler(void) {
	HAL_IncTick();
}

static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if (ret != HAL_OK) {
		while (1) { ; }
	}

	/* Activate the OverDrive to reach the 216 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if (ret != HAL_OK) {
		while (1) { ; }
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
								   RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if (ret != HAL_OK) {
		while (1) { ; }
	}
}
