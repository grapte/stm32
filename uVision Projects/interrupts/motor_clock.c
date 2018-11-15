#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include <stdio.h>

static void SystemClock_Config(void);
extern GLCD_FONT GLCD_Font_16x24;

int time = 0;
char servo_state = 0;

void EXTI0_IRQHandler() {}

int main() {
	char time_str[6] = {0};
	SystemClock_Config();
	
	// enable peripherals
	RCC->AHB1ENR |= 1 << 5 | 1 << 6 | 1 << 8; // enable GPIO F, G, I
	RCC->APB2ENR |= 1 << 18 | 1 << 14;  // enable TIM11, SYSCFG component
	RCC->APB1ENR |= 1 << 5; // enable TIM7
	NVIC_EnableIRQ(EXTI9_5_IRQn); // enable intrrupts for EXTI 6, 7
	NVIC_EnableIRQ(TIM7_IRQn); // enable intrrupts for TIM7
	
	// gpio setup
	GPIOI->MODER |= 1 << 4 | 1 << 6; // set PI2, PI3 to output
	GPIOF->MODER |= 2 << 14;// AF mode on PF7
	
	// timer setup
	GPIOF->AFR[0] |= 3 << 28; // select AFR7 to AF3(TIM11_CH1)
	TIM11->CCMR1 |= 7 << 4; // set OC1M to PWM2
	TIM11->CCER |= 1 << 0; // enable CH1
	
	// interrupts setup
	SYSCFG->EXTICR[1] |= 6 << 8 | 6 << 12; // set EXTI6 to PG6, EXTI7 to PG7
	EXTI->RTSR |= 1 << 6 | 1 << 7; // set rising edge trigger for EXTI 6, 7
	EXTI->FTSR |= 1 << 6 | 1 << 7; // set falling edge trigger for EXTI 6, 7
	EXTI->IMR |= 1 << 6 | 1 << 7; // unmask EXTI line 6, 7
	TIM7->DIER |= 1 << 0; // set TIM7 to generate interrupts
	
	// defaults for timer
	TIM7->PSC = 1799;
	TIM7->ARR = 59999;
	
	TIM11->PSC = 107;
	TIM11->ARR = 41499 + 1500; // min + half of full range = middle
	TIM11->CCR1 = 40000;
	
	TIM7->CR1 |= 1 << 0; // start timer
	TIM11->CR1 |= 1 << 0; // start timer
	
	GLCD_Initialize();
	
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	while (1) {
		sprintf(time_str, "%02d:%02d", time / 60, time % 60);
		GLCD_DrawString(50, 50, time_str);
	}
}

void TIM7_IRQHandler() {
	TIM7->SR = ~(1 << 0); // clear flag
	time++;
	switch (servo_state) {
		case 0:
			TIM11->ARR = 41499; // left most
			servo_state = 1;
			break;
		case 1:
			TIM11->ARR = 41499 + 1500; // center
			servo_state = 2;
			break;
		case 2:
			TIM11->ARR = 41499 + 1500 + 1500; // right most
			servo_state = 3;
			break;
		case 3:
			TIM11->ARR = 41499 + 1500; // center
			servo_state = 0;
			break;
	}
} 

void EXTI9_5_IRQHandler() {
	if ((EXTI->PR & 1 << 6) == 1 << 6) {
		EXTI->PR = 1 << 6; // clear flag
		GPIOI->ODR ^= 1 << 2; // toggle PI2
		GPIOI->ODR &= ~(1 << 3); // turn off PI3 
	} else if ((EXTI->PR & 1 << 7) == 1 << 7) {
		EXTI->PR = 1 << 7; // clear flag
		GPIOI->ODR |= 1 << 3; // turn on PI3
	}
}

void SysTick_Handler (void)
{
	HAL_IncTick();
}

static void SystemClock_Config(void)
{
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
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}
