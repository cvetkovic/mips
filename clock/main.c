#include <stdint.h>

#define RCC_APB2ENR 	(*((uint32_t*)(0x40021000 + 0x18)))

#define GPIOC_CRL		(*((uint32_t*)(0x40011000 + 0x00)))
#define GPIOC_CRH		(*((uint32_t*)(0x40011000 + 0x04)))
#define GPIOC_ODR		(*((uint32_t*)(0x40011000 + 0x0C)))

#define NVIC_ISER0		(*((uint32_t*)(0xE000E100 + 0x00)))
#define NVIC_ISER1		(*((uint32_t*)(0xE000E100 + 0x04)))

#define TIM1_CR1		(*((uint32_t*)(0x40012C00 + 0x00)))
#define TIM1_DIER		(*((uint32_t*)(0x40012C00 + 0x0C)))
#define TIM1_SR			(*((uint32_t*)(0x40012C00 + 0x10)))
#define TIM1_PSC		(*((uint32_t*)(0x40012C00 + 0x28)))
#define TIM1_ARR		(*((uint32_t*)(0x40012C00 + 0x2C)))

#define AFIO_EXTICR4	(*((uint32_t*)(0x40010000 + 0x14)))

#define EXTI_IMR		(*((uint32_t*)(0x40010400 + 0x00)))
#define EXTI_RTSR		(*((uint32_t*)(0x40010400 + 0x08)))
#define EXTI_PR			(*((uint32_t*)(0x40010400 + 0x14)))

uint8_t encoded[] = { 0x3F,
					  0x06,
					  0x5B,
					  0x4F,
					  0x66,
					  0x6D,
					  0x7D,
					  0x07,
					  0x7F,
					  0x67 };

uint8_t digits[4] = {0,0,0,1};	// nije inicijalizovano ovde jer nece da radi jer se verovatno alocira u .bss
uint8_t shownDigit = 0;

uint16_t ticks = 0;

void exti_13_handler()
{
	if ((EXTI_PR & 0x2000) != 0)
	{
		EXTI_PR = 0x2000;
		digits[0] = digits[1] = digits[2] = digits[3] = 0;
		ticks = 0;
	}
}

void incrementTime()
{
	if (digits[3] == 9)
	{
		digits[3] = 0;
		
		if (digits[2] == 9)
		{
			digits[2] = 0;
			
			if (digits[1] == 9)
			{
				digits[1] = 0;
				
				if (digits[0] == 9)
					digits[0] = 0;
				else
					digits[0]++;
			}
			else
				digits[1]++;
		}
		else
			digits[2]++;
	}
	else
		digits[3]++;
		
}

void timer_1_update_handler()
{	
	uint8_t cathodeSelector;
	if (shownDigit == 0)
		cathodeSelector = 0xE;
	else if (shownDigit == 1)
		cathodeSelector = 0xD;
	else if (shownDigit == 2)
		cathodeSelector = 0xB;
	else if (shownDigit == 3)
		cathodeSelector = 0x7;
	else
		cathodeSelector = 0xF;
	
	uint8_t led = 0x0;
	if (ticks > 50)
		led = 0x1;
	
	ticks++;
	if (ticks == 100)
	{
		ticks = 0;
		incrementTime();
	}
	
	GPIOC_ODR = (led << 12) | (cathodeSelector << 8) | encoded[digits[shownDigit]];
	shownDigit = (shownDigit + 1) % 4;
}

void main()
{
	digits[0] = digits[1] = digits[2] = digits[3] = 0;
	
	RCC_APB2ENR = (1 << 11) | (1 << 4) | 1;	// timer, ioportc, afio
	GPIOC_CRL = 0x22222222;		// sve output
	GPIOC_CRH &= 0xFF000000;
	GPIOC_CRH |= 0x00822222;	// 13-input, ostalo output
	
	AFIO_EXTICR4 = 0x0020;		// exti13 alternativna funkcija
	NVIC_ISER0 = (1 << 25);		// tajmer
	NVIC_ISER1 = 0x100;			// exti13 nvic enable
	
	TIM1_PSC = 7999;		// 1kHz
	TIM1_ARR = 10;			// 1000 ms
	TIM1_DIER = 1;
	TIM1_SR = 0;
	TIM1_CR1 = 1;
		
	EXTI_IMR = 0x2000;
	EXTI_RTSR = 0x2000;
	EXTI_PR = 0x2000;
	
	while (1);
}