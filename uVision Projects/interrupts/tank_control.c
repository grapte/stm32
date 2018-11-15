#include "stm32f7xx.h"                  // Device header

int main() {
	RCC->AHB1ENR |= 1 << 0 | 1 << 1; // activate GPIO A, B
	RCC->APB2ENR |= 1 << 14 | 1 << 17; // activate SYSCFG component, TIM10
	RCC->APB1ENR |= 1 << 5; // enable TIM7
	GPIOB->MODER &= ~ (2 << 6); // clear alternate funciton on pin 3
	GPIOB->MODER |= 0x55; // set to output on pin 0, 1, 2, 3
	GPIOF->MODER |= 2 << 12; // AF mode on pin 6
	GPIOF->AFR[0] |= 3 << 24; // AF3 to pin 6
	TIM7->PSC = 15999;
	TIM7->ARR = 59999;
	TIM7->DIER |= 1 << 0;
	TIM10->ARR = 49999;
	TIM10->CCMR1 |= 4 << 4;
	TIM10->CCER |= 1 << 0;
	EXTI->IMR |= 1 << 3;
	EXTI->RTSR |= 1 << 3;
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_IRQHandler() {
	EXTI->PR = 1 << 3;
	if ((GPIOA->IDR & 1 << 3) == 0x00) {
		GPIOB->BSRR = 0x0B;
		TIM10->PSC = 159;
		TIM10->CCMR1 = 3 << 4;
		TIM10->CR1 |= 1 << 0;
		NVIC_EnableIRQ(TIM7_IRQn);
		TIM7->CR1 = 0;
		EXTI->IMR |= 1 << 1;
		EXTI->RTSR |= 1 << 1;
		NVIC_EnableIRQ(EXTI1_IRQn);
		EXTI->IMR &= ~ (1 << 3);
	}
}

void TIM7_IRQHandler() {
	TIM7->SR = ~(1 << 0);
	TIM7->CR1 &= ~(1 << 0);
	TIM7->CNT = 0;
	NVIC_DisableIRQ(TIM7_IRQn);
	GPIOB->BSRR = 1 << 19;
}