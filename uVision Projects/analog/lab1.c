#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include <stdio.h>

static void SystemClock_Config(void);
extern GLCD_FONT GLCD_Font_16x24;

double volts = 0;
int N = 0;
char s = 0;

int main(void) {
	SystemClock_Config();
	
	char print_str[15];
	

	RCC->AHB1ENR |= 1 << 0 | 1 << 5; // enable gpio A, F
	RCC->APB1ENR |= 1 << 5; // enable TIM7
	RCC->APB2ENR |= 1 << 10; // enable ADC3
	RCC->APB1ENR |= (1 << 29); // enable DAC
	NVIC_EnableIRQ(TIM7_IRQn); // enable intrrupts for TIM7
	
	GPIOF->MODER |= 3 << 12 | 3 << 20; // set PF6, PF10 to analog
	GPIOA->MODER |= 3 << 8; // set PA4 to analog
	
	ADC3->SQR3 |= 4 << 0; // decode from IN4
	
	TIM7->DIER |= 1 << 0; // set TIM7 to generate interrupts
	
	TIM7->PSC = 1799;
	TIM7->ARR = 5;
	
	ADC3->CR2 |= 1 << 0; // start decoder
	DAC->CR |= (1 << 0); // start encoder
	TIM7->CR1 |= 1 << 0; // start timer
	
	GLCD_Initialize();
	
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	while (1) {
		sprintf(print_str, "%04.2fV", volts);
		GLCD_DrawString(50, 50, print_str);
	}
}

void TIM7_IRQHandler() {
	TIM7->SR = ~(1 << 0); // clear flag
	
	ADC3->SQR3 = 4 << 0; // decode from IN4	
	ADC3->CR2 |= 1 << 30; // start decoding
	while ((ADC3->SR & (1 << 1)) == 0);
	N = (ADC3->DR - 2048) * 2 + 2048;
	if (N > 4095) N = 4095;
	if (N < 0) N = 0;
	DAC->DHR12R1 = N; // sends value to encode

	ADC3->SQR3 = 8 << 0; // decode from IN8	
	ADC3->CR2 |= 1 << 30; // start decoding
	while ((ADC3->SR & (1 << 1)) == 0);
	volts = (ADC3->DR / 4096.0) * 3.3;

	
	
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
