#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD
#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include <stdlib.h>


static void SystemClock_Config(void);
extern GLCD_FONT GLCD_Font_16x24;

int main(void)
{
	short x, y;
	TOUCH_STATE state;
	
	SystemClock_Config();
	
	RCC->AHB1ENR |= 1 << 5; // enable GPIO F
	RCC->APB2ENR |= 1 << 17 | 1 << 18; // enable TIM10 TIM11
	
	GPIOF->MODER |= 2 << 12 | 2 << 14; // AF mode on PF6 PF7
	
	GPIOF->AFR[0] |= 3 << 24 | 3 << 28; // select AFR6 to AF3(TIM10_CH1), AFR7 to AF3(TIM11_CH1)
	
	TIM10->CCMR1 |= 7 << 4; // set OC1M to PWM2
	TIM11->CCMR1 |= 7 << 4; // set OC1M to PWM2
	
	TIM10->CCER |= 1 << 0; // enable CH1
	TIM11->CCER |= 1 << 0; // enable CH1
	
	TIM10->CR1 |= 1 << 0; // start timer
	TIM11->CR1 |= 1 << 0; // start timer
	
	TIM10->PSC = 71;
	TIM10->ARR = 63899 + 600; // min + half = middle (stationary)
	TIM10->CCR1 = 60000;
	
	TIM11->PSC = 107;
	TIM11->ARR = 41499 + 1500; // min + half of full range = middle
	TIM11->CCR1 = 40000;
	
	GLCD_Initialize();
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawHLine(0, GLCD_SIZE_Y / 2, GLCD_SIZE_X);
	GLCD_DrawVLine(GLCD_SIZE_X / 2, 0, GLCD_SIZE_Y);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW);
	
	Touch_Initialize();
	
	x = GLCD_SIZE_X / 2;
	y = GLCD_SIZE_Y / 2;
	
	GLCD_DrawPixel(x, y);
	while(1)
	{
		Touch_GetState(&state);
		
		if (state.pressed == 1)
		{
			GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
			GLCD_DrawPixel(x, y);
			
			x = state.x;
			y = state.y;
			GLCD_SetForegroundColor(GLCD_COLOR_YELLOW);
			GLCD_DrawPixel(x, y);
			
			TIM10->ARR = 63899 + 600 + (136 - y) * 5;  // y has a maximum of 272, 5 > 1200/262
			TIM11->ARR = 41499 + 1500 + (240 - x) * 7; // x has a maximum of 480, 7 > 3000/480
		}
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
