#include "stm32f7xx.h"                  // Device header

int main() {
	RCC->AHB1ENR |= 1 << 6; // enable GPIO G
	RCC->APB1ENR |= 1 << 5; // enable TIM7
	GPIOG->MODER |= 1 << 14; // set PG6 to output
	
	// PSC*ARR =  8000000
	TIM7->PSC = 199;
	TIM7->ARR = 39999;
	
	TIM7->CR1 |= 1 << 0; // start TIM7
	
	TIM7->DIER = 1 << 0; // enable update IRQ
	
	NVIC_EnableIRQ(TIM7_IRQn); // enable IRQ in NVIC
}

void TIM7_IRQHandler() {
	EXTI->PR = 1 << 0;
	GPIOG->ODR ^= 1 << 7;
}