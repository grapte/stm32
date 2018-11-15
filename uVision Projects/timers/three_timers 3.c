//three_timers.c

#include "stm32f7xx.h"                  // Device header

int main() {
	RCC->AHB1ENR |= 1 << 2 | 1 << 8; // activate gpio C and I
	RCC->APB1ENR |= 1 << 1 | 1 << 3; // activate timer TIM3 and TIM5
	RCC->APB2ENR |= 1 << 1; // activate timer TIM8
	GPIOI->MODER |= 2 << 0; // PI0 to Alternative Function
	GPIOC->MODER |= 2 << (6 * 2) | 2 << (7 * 2); // PC6 and PC7 to AF
	GPIOI->AFR[0] |= 2 << 20; // PI0 to AF2 
	GPIOC->AFR[0] |= 2 << 24 | 3 << 28; // PC6 to AF2, PC7 to AF3
	TIM5->CCMR2 |= 3 << 12; // change OC4M to toggle
	TIM5->CCER |= 1 << 12; // connect OC4 to TIM5_CH4
	TIM3->CCMR1 |= 3 << 4; // change OC1M to toggle
	TIM3->CCER |= 1 << 0; // connect OC1M to TIM3_CH1
	TIM5->PSC = 249; // prescaler factor
	TIM5->ARR = 39999; // automatic reload on
	TIM3->PSC = 249; // prescaler factor
	TIM3->ARR = 39999; // automatic reload on
	TIM5->CCR1 = 0; // change phase shift to 0
	TIM3->CCR1 = 0; // change phase shift to 0
	TIM5->CR1 |= 1 << 0; // clock enable
	TIM3->CR1 |= 1 << 0; // clock enable
}
