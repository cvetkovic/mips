#include <stdint.h>

#define RCC_APB2ENR			(*((uint32_t*)(0x40021000 + 0x18)))

#define AFIO_EXTICR4		(*((uint32_t*)(0x40010000 + 0x14)))

#define NVIC_ISER0			(*((uint32_t*)(0xE000E100 + 0x00)))
#define NVIC_ISER1			(*((uint32_t*)(0xE000E100 + 0x04)))

#define GPIOC_CRH			(*((uint32_t*)(0x40011000 + 0x04)))
#define GPIOC_CRL			(*((uint32_t*)(0x40011000 + 0x00)))
#define GPIOC_ODR			(*((uint32_t*)(0x40011000 + 0x0C)))

#define TIM1_CR1			(*((uint32_t*)(0x40012C00 + 0x00)))
#define TIM1_SR				(*((uint32_t*)(0x40012C00 + 0x10)))
#define TIM1_ARR			(*((uint32_t*)(0x40012C00 + 0x2C)))
#define TIM1_PSC			(*((uint32_t*)(0x40012C00 + 0x28)))
#define TIM1_DIER			(*((uint32_t*)(0x40012C00 + 0x0C)))

#define EXTI_IMR 			(*((uint32_t*)(0x40010400 + 0x00)))
#define EXTI_RTSR			(*((uint32_t*)(0x40010400 + 0x08)))
#define EXTI_PR				(*((uint32_t*)(0x40010400 + 0x14)))

uint8_t encoding[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
uint8_t digits[] = {0,0,0,0};
uint8_t shownDigit = 0;
uint8_t time = 0;

void ext_handler()
{
	if ((EXTI_PR & 0x2000) != 0)
	{
		EXTI_PR |= 0x2000;
		//TIM1_CNT = 10; // OVDE TREBA DA SE COUNTER TIMER-A RESETUJE
		
		for (int i = 0; i < 4; i++)
			digits[i] = 0;
	}
}

void incrementDigit()
{
	if (digits[0] == 9)
	{
		digits[0] = 0;
		
		if (digits[1] == 9)
		{
			digits[1] = 0;
			
			if (digits[2] == 9)
			{
				digits[2] = 0;
				
				if (digits[3] == 9)
				{
					digits[0] = 0;
					digits[1] = 0;
					digits[2] = 0;
					digits[3] = 0;
				}
				else
					digits[3]++;
			}
			else
				digits[2]++;
		}
		else
			digits[1]++;
	}
	else
		digits[0]++;
}

void timer_handler()
{
	if ((TIM1_SR & 0x01) != 0)
	{
		TIM1_SR = 0;
		
		uint8_t cathodeSelector;
		if (shownDigit == 0)
			cathodeSelector = 0x7;
		else if (shownDigit == 1)
			cathodeSelector = 0xB;
		else if (shownDigit == 2)
			cathodeSelector = 0xD;
		else if (shownDigit == 3)
			cathodeSelector = 0xE;
		else
			cathodeSelector = 0xF;
		
		time++;
		if (time == 99)
		{
			time = 0;
			incrementDigit();
		}
		
		GPIOC_ODR = (cathodeSelector << 8) | encoding[digits[shownDigit]];
		shownDigit = (shownDigit + 1) % 4;
	}
}

void main()
{
	RCC_APB2ENR = (1 << 11) | (1 << 4) | 1;
	AFIO_EXTICR4 = 0x20;
	
	NVIC_ISER0 = 1 << 25;
	NVIC_ISER1 = 1 << 8;
	
	GPIOC_CRL = 0x22222222;
	GPIOC_CRH &= 0xFF000000;
	GPIOC_CRH |= 0x00822222;
	
	TIM1_SR = 0;
	TIM1_DIER = 1;
	TIM1_PSC = 7999;
	TIM1_ARR = 10;
	TIM1_CR1 = 1;
	
	EXTI_IMR = 1 << 13;
	EXTI_RTSR = 1 << 13;
	EXTI_PR = 0;
	
	while (1);
}