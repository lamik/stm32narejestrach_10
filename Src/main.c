/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mateusz Salamon
 * @brief          : STM32 na Rejestrach
 ******************************************************************************
 ******************************************************************************

 	 	 	 	 https://msalamon.pl
 	 	 	 	 https://sklep.msalamon.pl
 	 	 	 	 https://kursstm32.pl
 	 	 	 	 https://stm32narejestrach.pl

 */
#include "main.h"

// LED LD4 macros
#define LD4_ON GPIOA->BSRR = GPIO_BSRR_BS5
#define LD4_OFF GPIOA->BSRR = GPIO_BSRR_BR5;

// Constants for Software Timer's actions

// Tick for System Time
__IO uint32_t Tick;

// LED LD4 configuration
void LD4_Configure(void);

// ADC functions
void ADC_Configure(void);
void ADC_Input0GpioConfigure(void);
void ADC_ChannelSelection(void);

void ADC_Conversion(void);
uint32_t ADC_ReadData(void);

// Get current System Time
uint32_t GetSystemTick(void);

uint32_t AdcSample; // Temporary variable for ADC Data

int main(void)
{
// 1s = 12 000 000
	// 0,001 = X
	SysTick_Config(12000000 / 1000);

	LD4_Configure();
	ADC_Configure();
	ADC_Input0GpioConfigure();
	ADC_ChannelSelection();

    /* Loop forever */
	while(1)
	{
		ADC_Conversion();
		AdcSample = ADC_ReadData();
	}
}

void ADC_Configure(void)
{
	// RCC Clocks
	RCC->APBENR2 |= RCC_APBENR2_ADCEN;

	// Clock prescaler (default - no divider)
	// Data alignment (defalut - Right)
	// Data resolution (default - 12 bit)
	// Single Conversion (default)
	// Software trigger (default)

	// ADC Voltage Regulator On
	ADC1->CR |= ADC_CR_ADVREGEN;

	// Sampling Time (default - 1.5 ADC clock cycles)
	ADC1->SMPR |= ADC_SMPR_SMP1_0 | ADC_SMPR_SMP1_1 | ADC_SMPR_SMP1_2; // Set to 160.5 clock cycles

	// Calibration
	ADC1->CR |= ADC_CR_ADCAL;
	while(ADC1->CR & ADC_CR_ADCAL); // Wait for calibration

	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
}

void ADC_Input0GpioConfigure(void)
{
	// PA0
	// Enable Clock for PORTA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Configure GPIO Mode - Analog
	// Defalut mode is analog
}

void ADC_ChannelSelection(void)
{
	// Sequence mode selected as all selected channels as active input
	// ADC1->CFGR1 &= ~(CHSELRMOD); // default

	// Select channel
	ADC1->CHSELR |= ADC_CHSELR_CHSEL0;
}

void ADC_Conversion(void)
{
	// Start ADC Conversion
	ADC1->CR |= ADC_CR_ADSTART;
	// Wait for finish
	while(!(ADC1->ISR & ADC_ISR_EOC));
}

uint32_t ADC_ReadData(void)
{
	return ADC1->DR;
}

void LD4_Configure(void)
{
	// Enable Clock for PORTA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Configure GPIO Mode - Output
	GPIOA->MODER |= GPIO_MODER_MODE5_0; // It's default reset state. Not necessary.
	GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);

	// Configure Output Mode - Push-pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5); // It's default reset state. Not necessary.

	// Configure GPIO Speed - Low
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5); // Two bits together. It's default reset state. Not necessary.

	// Configure Pull-up/Pull-down - no PU/PD
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); // It's default reset state. Not necessary.
}

void SysTick_Handler(void)
{
	Tick++; // Increase system timer
}

uint32_t GetSystemTick(void)
{
	return Tick; // Return current System Time
}

