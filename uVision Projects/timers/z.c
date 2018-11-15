//three_timers.c

#include "stm32f7xx.h"                  // Device header

int main() {
	RCC->AHB1ENR |= 1 << 2 | 1 << 8; // activate gpio C and I
	RCC->APB1ENR |= 1 << 1 | 1 << 3; // activate timer TIM3 and TIM5
	RCC->APB2ENR |= 1 << 1; // activate timer TIM8
	GPIOI->MODER |= 2 << 0; // PI0 to Alternative Function
	GPIOC->MODER |= 2 << (6 * 2) | 2 << (7 * 2); // PC6 and PC7 to AF
	GPIOI->AFR[0] |= 0 << 8; // map AF2 (TIM5) to PI0
	GPIOC->AFR[0] |= 2 << 8 | 3 << 12; // map AF2(TIM3) to PC6, AF3(TIM8) to PC7
	TIM5->CCMR1 |= 3 << 4; // change OC1 to toggle
	TIM5->CCER |= 1 << 0; // change OC1 to active
	TIM5->PSC = 249; // prescaler factor
	TIM5->ARR = 39999; // automatic reload on
	TIM5->CCR1 = 0; // change phase shift to 0
	TIM5->CR1 |= 1 << 0;
}
