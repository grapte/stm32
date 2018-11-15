#include "stm32f7xx.h"                  // Device header

int main () {
	RCC->AHB1ENR |= 1 << 1 | 1 << 5 | 1 << 8; // enable GPIO B F I
	RCC->APB1ENR |= 1 << 6 | 1 << 7; // enable TIM12 TIM13
	
	GPIOB->MODER |= 2 << 28| 2 << 30; // AF mode on PB14 PB15
	GPIOF->MODER |= 2 << 16; // AF mode on PF8
	
	GPIOB->AFR[1] |= 9 << 24 | 9 << 28; // select AFR14 to AF9(TIM12_CH1), AFR15 to AF9(TIM12_CH2)
	GPIOF->AFR[1] |= 9 << 0; // select AFR8 to AF9(TIM13_CH1)
	
	TIM12->CCMR1 |= 3 << 4 | 6 << 12; // set OC1M to toggle, set OC2M to PWM1
	TIM13->CCMR1 |= 6 << 4; // set OC1M to PWM1
	
	TIM12->CCER |= 1 << 0 | 1 << 4; // enable CH1 CH2
	TIM13->CCER |= 1 << 0; // enable CH1
	
	TIM12->PSC |= 8 - 1;
	TIM12->ARR |= 40000 - 1; // CH1 is 25 Hz, CH2 is 50Hz PWM.
	
	TIM13->PSC = 0;
	TIM13->ARR = 800 - 1; // TIM3_CH1 is 20 Hz
	
	TIM12->CR1 |= 1 << 0; // enable timer
	TIM13->CR1 |= 1 << 0; // enable timer
	
	while (1) {
		TIM13->CCR1 = (GPIOI->IDR & 0x3) * 200; // Numerically PI[1:0] determines the duty cycle
	}
}
