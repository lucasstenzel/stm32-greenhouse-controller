#include "delay.h"

// Forward declaration of helper functions
static void delay_1ms(void);
static void delay_1us(void);

void delay_ms(uint32_t n){
	for (uint32_t i = 0; i < n; i++) {
		delay_1ms();
	}
}

void delay_us(uint32_t n){
	for (uint32_t i = 0; i < n; i++) {
		delay_1us();
	}
}

static void delay_1ms(void)
{
    // Load number of cycles; 16 MHz clock
    *(STK_LOAD) = 16000;
    // Clear current value
    *(STK_VAL) = 0;
    // Enable SysTick and set clock source
    *(STK_CTRL) = (1 << EN) | (1 << CLKSOURCE);
    
    // Wait for countflag to be set
    while ((*(STK_CTRL) & (1 << COUNTFLAG)) == 0);

    // Disable SysTick
    *(STK_CTRL) = 0;
}

static void delay_1us(void)
{
    // Load number of cycles; 16 MHz clock
    *(STK_LOAD) = 16;
    // Clear current value
    *(STK_VAL) = 0;
    // Enable SysTick and set clock source
    *(STK_CTRL) = (1 << EN) | (1 << CLKSOURCE);
    
    // Wait for countflag to be set
    while ((*(STK_CTRL) & (1 << COUNTFLAG)) == 0);

    // Disable SysTick
    *(STK_CTRL) = 0;
}