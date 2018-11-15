// my_utils.c
#include "stm32f7xx.h"                  // Device header
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "my_utils.h"

void activate_peripheral(uint gpio) {
	RCC->AHB1ENR |= 1 << gpio;
}

void set_register_mode(uint gpio, uint pin, uint mode) {
	GPIO_TypeDef  *GPIOx;
	GPIOx = (GPIO_TypeDef *) (GPIOA_BASE + 0x400 * gpio);
	if ((gpio == A && (pin == 15 || pin == 14 || pin == 13)) || 
		  (gpio == B && (pin == 4 || pin == 3))) {
		GPIOx->MODER &= ~(0x3 << pin * 2);
	}
			
	GPIOx->MODER |= mode << pin * 2;
}

void activate_led(uint gpio, uint pin) {
	GPIO_TypeDef  *GPIOx;
	GPIOx = (GPIO_TypeDef *) (GPIOA_BASE + 0x400 * gpio);
	GPIOx->MODER |= 1 << pin * 2;
	GPIOx->ODR   |= 1 << pin;
}

void SysTick_Handler(void) {
	HAL_IncTick();
}

void SystemClock_Config(void) {
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
