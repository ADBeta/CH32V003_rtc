/******************************************************************************
* (c) ADBeta 2024
******************************************************************************/
#include "ch32v003fun.h"
#include <stdio.h>

#define RTC_TICKS_PER_SEC 32768
volatile uint32_t _rtc_ticks = 0x0000;
volatile uint32_t _rtc_secs  = 0x0000;

// External Interrupt IRQ Handler
void EXTI7_0_IRQHandler(void) __attribute__((interrupt));
void EXTI7_0_IRQHandler(void) 
{
	// If PD4 triggers, incriment the ticks counter.
	// If this counter wraps, reset and incriment seconds
	if(EXTI->INTFR & EXTI_Line4)
	{
		++_rtc_ticks;

		// This just incriments seconds, which is not very useful.
		// Replace with time managment function if needed. 
		// NOTE: Each tick at 32.768KHz has 1464 cycles free
		if(_rtc_ticks >= RTC_TICKS_PER_SEC)
		{
			_rtc_ticks = 0;
			++_rtc_secs;
		}
		
		// Acknowledge the interrupt
		EXTI->INTFR = EXTI_Line4;
	}
}


int main()
{
	SystemInit();
	
	// Enable PORTD for the XTAL, and AFIO for Interrupts
	RCC->APB2PCENR = RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO;
	// Set up PD4 as a floating input for the XTAL to pulse
	funPinMode(PD4, GPIO_CFGLR_IN_FLOAT);

	// Configure PD4 as an interrupt
	AFIO->EXTICR |= AFIO_EXTICR_EXTI4_PD;
	// Enable EXT4 interrupt
	EXTI->INTENR |= EXTI_INTENR_MR4;
	// Set EXT4 to trigger on rising edge
	EXTI->RTENR |= EXTI_RTENR_TR4;

	// Enable External Interrputs
	NVIC_EnableIRQ(EXTI7_0_IRQn);

	while(1)
	{
		Delay_Ms(1000);
		printf("%d\n", _rtc_ticks);
	}

}
