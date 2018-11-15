#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include <stdlib.h>
#include <string.h>

/* USART Driver */
extern ARM_DRIVER_USART Driver_USART7;
extern GLCD_FONT GLCD_Font_16x24;

static void SystemClock_Config(void);

char rg, gb;
short rgb;

void draw_pixel(int x, int y) {
	Driver_USART7.Receive(&rg, 1);
	while(Driver_USART7.GetRxCount() != 1);
	Driver_USART7.Receive(&gb, 1);
	while(Driver_USART7.GetRxCount() != 1);
	rgb = (rg << 8) | gb;
	GLCD_SetForegroundColor(rgb);
	GLCD_DrawPixel(x, y);
}

int main(void) {
	SystemClock_Config();
	GLCD_Initialize();
	Driver_USART7.Initialize(NULL);
	
	Driver_USART7.PowerControl(ARM_POWER_FULL);
	Driver_USART7.Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 115200);
	Driver_USART7.Control (ARM_USART_CONTROL_TX, 1);
	Driver_USART7.Control (ARM_USART_CONTROL_RX, 1);
	
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	Driver_USART7.Send("Hello World!\n\r", 14);
	
	char recv[256] = {0}, fmtdisp_str[32] = {0}, direction = 0;
	short mode = 0;
	
	while(1) {
		switch(mode) {
			case 0:
				// hello world! -> hello device
				Driver_USART7.Receive(&recv, 12);
				while(Driver_USART7.GetRxCount() != 12) {
					sprintf(fmtdisp_str, "RX Count: %02i", Driver_USART7.GetRxCount());
					GLCD_DrawString(50, 75, fmtdisp_str);
				}
//			Driver_USART7.Send(recv, 80);
				recv[13] = '\0';
				GLCD_DrawString(50, 100, recv);
				if (strcmp(recv, "go to next 1") == 0) {
					GLCD_ClearScreen();
					mode = 0x110;
				} else {
//					GLCD_ClearScreen();
//					GLCD_DrawString(50, 50, "Ending.");
//					exit(0);
				}
				break;
			case 1 ... 0xfe:
//				Driver_USART7.Receive(&mode, 2);
//				if (mode == 0xff) {
//					mode = 0x100;
//					break;
//				}
//				recv[mode] = '\0';
//				Driver_USART7.Receive(&recv, mode);
//				GLCD_DrawString()
//				Driver_USART7.Send(recv, mode);
				break;
			case 0x100:
//				mode = 110;
//				Driver_USART7.Receive(&direction, 1);
//				while(Driver_USART7.GetRxCount() != 1);
//				switch (direction) {
//					case 1:
//						mode = 0x110;
//						break;
//					case 2:
//						mode = 0x111;
//						break;
//				}
				break;
			case 0x110:
				for (int y = 0; y < 272; y++) {
//					if (y % 20 == 0)
//						Driver_USART7.Send("line", 4);
					for (int x = 0; x < 480; x++) {
						draw_pixel(x, y);
					}
				}
				mode = 0x110;
				break;
			case 0x111:
				for (int y = 271; y > -1; y--) {
//					if (y % 20 == 0)
//						Driver_USART7.Send("line", 4);
					for (int x = 479; x > -1; x--) {
						draw_pixel(x, y);
					}
				}
				mode = 0x111;
				break;
			default:
				break;
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
