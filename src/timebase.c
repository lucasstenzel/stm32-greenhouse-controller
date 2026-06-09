#include "gpio.h"
#include "timebase.h"

// Struct pointers
static volatile TIMER* TIM6 = (TIMER*)0x40001000;

#define TIM6_EN     4   // TIM6EN bit in RCC_APB1ENR

#define NVIC_ISER1  (volatile uint32_t*) 0xE000E104     // Enables IRQ 32-63
#define TIM6_DAC_IRQ_BIT 22     // IRQ 54 (22nd bit in ISER1)

// Timer bit positions (TIMx_CR1 / DIER / SR / EGR)
#define CR1_CEN     0   // Counter Enable
#define DIER_UIE    0   // Update Interrupt Enable
#define SR_UIF      0   // Update Interrupt Flag
#define ERG_UG      0   // Update Generation

/**
 * APB1 timer clock is 16MHz, so prescale to 1MHz and reload every 1000 ticks
 * to trigger the update event once per millisecond
 */
#define TIMEBASE_PSC ((16000000UL / 1000000UL) - 1)     // 16 MHz -> 1 MHz, = 15
#define TIMEBASE_ARR (1000UL - 1)                       // 1000 us = 1 ms period

// Software counter that extends the 1 ms tick into a 32-bit millisecond clock.
static volatile uint32_t g_ms = 0;

void timebase_init(void)
{
    // Enable the TIM6 peripheral clock
    *(RCC_APB1ENR) |= (1 << TIM6_EN);

    TIM6->PSC = TIMEBASE_PSC;    // 1 MHz tick
    TIM6->ARR = TIMEBASE_ARR;    // Reload every 1 ms

    TIM6->EGR |= (1 << ERG_UG);     // Force an update to load PSC/ARR
    TIM6->SR  &= ~(1 << SR_UIF);    // Clear the flag the UG event just set

    TIM6->DIER |= (1 << DIER_UIE);    // enable the update interrupt

    // Enable the TIM6_DAC interrupt line in the NVIC
    *(NVIC_ISER1) = (1 << TIM6_DAC_IRQ_BIT);

    TIM6->CR1 |= (1 << CR1_CEN);     // start counting
}

uint32_t millis(void)
{
    // 32-bit aligned read is a single LDR on Cortex-M4, so this is atomic with
    // respect to the ISR; no need to mask interrupts.
    return g_ms;
}

/**
 * @brief TIM6 update interrupt handler, triggers every 1 ms. This handler
 *        is shared by the DAC, but it's not being used here.
 *        (IRQHandler names are defined by startup_stm32f446xx.s)
 */
void TIM6_DAC_IRQHandler(void)
{
    if (TIM6->SR & (1 << SR_UIF))   // Confirm the update was from TIM6 and not the DAC
    {
        TIM6->SR &= ~(1 << SR_UIF); // Clear the flag
        g_ms++;
    }
}
