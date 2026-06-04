#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

// SysTick register addresses
#define STK_CTRL (volatile uint32_t*) 0xE000E010
#define STK_LOAD (volatile uint32_t*) 0xE000E014
#define STK_VAL (volatile uint32_t*) 0xE000E018

// SysTick control register bits
#define EN 0
#define TICKINT 1
#define CLKSOURCE 2
#define COUNTFLAG 16

void delay_ms(uint32_t n);
void delay_us(uint32_t n);

#endif /* DELAY_H_ */