/******************************************************************************
* (c) ADBeta 2024
******************************************************************************/
#include "ch32v003fun.h"
#include <stdio.h>

volatile uint16_t _rtc_ticks = 0x0000;
volatile uint16_t _rtc_secs  = 0x0000;

// External Interrupt IRQ Handler
void EXTI7_0_IRQHandler(void) __attribute__((interrupt));
void EXTI7_0_IRQHandler(void) 
{
	// If PD4 triggers, incriment the ticks counter.
	// If this counter wraps to 0x0000, incriment seconds
	if(EXTI->INTFR & EXTI_Line4)
	{

		// Acknowledge the interrupt
		EXTI->INTFR = EXTI_Line4;
	}
}


int main()
{
	SystemInit();

	// NOTE: PD0 LED
	
	// Enable PORTD for the XTAL, and AFIO for Interrupts
	RCC->APB2PCENR = RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO;
	// Set up PD4 as a floating input for the XTAL to pulse
	//funPinMode(PD4, GPIO_CFGLR_IN_FLOAT);
	funPinMode(PD4, GPIO_CFGLR_IN_PUPD);
	funDigitalWrite(PD4, FUN_LOW);

	funPinMode(PD0, GPIO_CFGLR_OUT_10Mhz_PP);

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
	}

}
