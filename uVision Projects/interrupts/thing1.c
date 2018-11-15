#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include <stdlib.h>

static void SystemClock_Config(void);

int main(void) {
	SystemClock_Config();

	RCC->AHB1ENR |= 1 << 0 | 1 << 8; // activate GPIO A, I
	RCC->APB2ENR |= 1 << 14; // enable SYSCFG component
	GPIOI->MODER |= 1 << 0; // set PI0 to output
	SYSCFG->EXTICR[0] &= ~0x0f; // set EXTI to PA0
	EXTI->IMR |= 1 << 0; // enable EXTI line 1
	EXTI->RTSR |= 1 << 0; // rising edge trigger
	NVIC_EnableIRQ(EXTI0_IRQn); // enable IRQ in flashing step?
	
	GLCD_Initialize();
	
	while(1) {
		GLCD_SetBackgroundColor(rand() % 255);
		GLCD_ClearScreen();
		for(int i = 0; i < 1000000; i++);
	}

	return 0;
}

void EXTI0_IRQHandler() {
	EXTI->PR = 1 << 0;
	GPIOI->ODR ^= 1 << 0;
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
