#include "polling_adc.h"

namespace polling_adc {

static constexpr int ADC_CHAN = 0; // ADC chan 0
static constexpr int ADC_APIN = 2; // A2

/*
 * initialize adc for polling
 */
void init( void )
{
	// ADCCLK = 24 MHz => RCC_ADCPRE = 0: divide by 2
	RCC->CFGR0 &= ~(0x1F<<11);
	
	// Enable GPIOD and ADC
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_ADC1;
	
 	// PA2 is analog input channel 0 (A0): CH32V003DS0 page 11 2.2 Pin Description
	// CH32V003RM page 57 7.3.1.1 Port Configuration Register Low
        GPIOA->CFGLR &= ~(0xf << (4 * ADC_APIN)); 	// CNF = 00: Analog input mode, Mode = 00: Input mode
	
	// Reset the ADC to init all regs
	RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;
	
	// Set up single conversion on ADC_CHAN
	ADC1->RSQR1 = 0;
	ADC1->RSQR2 = 0;
	ADC1->RSQR3 = ADC_CHAN;	// 0-9 for 8 ext inputs and two internals
	
	// set sampling time for ADC_CHAN
	ADC1->SAMPTR2 &= ~(ADC_SMP0 << (3 * ADC_CHAN));
	ADC1->SAMPTR2 |= 7 << (3 * ADC_CHAN);	// 0:7 => 3/9/15/30/43/57/73/241 cycles
		
	// turn on ADC and set rule group to sw trig
	ADC1->CTLR2 |= ADC_ADON | ADC_EXTSEL;
	
	// Reset calibration
	ADC1->CTLR2 |= ADC_RSTCAL;
	while(ADC1->CTLR2 & ADC_RSTCAL);
	
	// Calibrate
	ADC1->CTLR2 |= ADC_CAL;
	while(ADC1->CTLR2 & ADC_CAL);
	
	// should be ready for SW conversion now
}

/*
 * start conversion, wait and return result
 */
uint16_t get( void )
{
	// start sw conversion (auto clears)
	ADC1->CTLR2 |= ADC_SWSTART;
	
	// wait for conversion complete
	while(!(ADC1->STATR & ADC_EOC));
	
	// get result
	return ADC1->RDATAR;
}

}