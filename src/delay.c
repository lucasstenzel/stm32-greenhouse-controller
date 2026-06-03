#include <inttypes.h>
#include "delay.h"  

void delay_ms(uint32_t n){
	for (int i = 0; i < n; i++) {
		delay_1ms();
	}
}

void delay_us(uint32_t n){
	for (int i = 0; i < n; i++) {
		delay_1us();
	}
}

void delay_1ms(void)
{
    // Load number of cycles; 16 MHz clock
    *(STK_LOAD) = 16000;
    // Clear current value
    *(STK_VAL) = 0;
    // Enable SysTick and set clock source
    *(STK_CTRL) = (1 << EN) | (1 << CLKSOURCE);
    
    // Wait for countflag to be set
    while (*(STK_CTRL) & (1 << COUNTFLAG) == 0);

    // Disable SysTick
    *(STK_CTRL) = 0;
}

void delay_1us(void)
{
    // Load number of cycles; 16 MHz clock
    *(STK_LOAD) = 16;
    // Clear current value
    *(STK_VAL) = 0;
    // Enable SysTick and set clock source
    *(STK_CTRL) = (1 << EN) | (1 << CLKSOURCE);
    
    // Wait for countflag to be set
    while (*(STK_CTRL) & (1 << COUNTFLAG) == 0);

    // Disable SysTick
    *(STK_CTRL) = 0;
}