#include "gpio.h"
#include "fan.h"

// Struct pointers
static volatile GPIO* GPIOB = (GPIO*)0x40020400;    // Using PB4 for the fan PWM output
static volatile GPIO* GPIOA = (GPIO*)0x40020000;    // Using PA0 for the tach input
static volatile TIMER* TIM3 = (TIMER*)0x40000400;   // PWM output (APB1 bus)
static volatile TIMER* TIM2 = (TIMER*)0x40000000;   // Tach input capture (APB1 bus)

#define FAN_PWM_PIN 4    // PB4
#define FAN_PWM_AF  2    // TIM3_CH1 alternate function

#define TACH_PIN 0       // PA0
#define TACH_AF  1       // TIM2_CH1 alternate function

/** 
 * The APB1 timer clock is at 16MHz using the High-Speed Internal oscillator (HSI)
 * with no PLL and no Prescaler.
 * To reach 25kHz PWM frequency, we need 16MHz / 25kHz = 640 timer ticks per period
 */
#define FAN_PWM_PERIOD 640
#define FAN_PWM_ARR    (FAN_PWM_PERIOD - 1)

/**
 * TIM2 is prescaled to a 1 MHz tick, so a capture in
 * CCR1 corresponds to the pulse period (in microseconds).
 *
 * A 4-wire fan emits 2 tach pulses per revolution, so:
 *   RPM = 60 * (pulses_per_sec / 2) = 60/2 * tick_rate / period_ticks
 */
#define TACH_TICK_HZ       1000000UL
#define TACH_PRESCALE           ((16000000UL / TACH_TICK_HZ) - 1)   // 16 MHz -> 1 MHz, = 15
#define TACH_RPM_FACTOR     60UL / 2UL   // 60 seconds per minute, 2 pulses per revolution

void fan_init(void)
{
    // Enable GPIOB peripheral clock
    *(RCC_AHB1ENR) |= (1 << GPIOBEN);

    GPIOB->MODER &= ~(0x3 << (FAN_PWM_PIN * 2));    // Clear the two MODER bits for PB4
    GPIOB->MODER |=  (ALTERNATE_FUNCTION << (FAN_PWM_PIN * 2)); // Set PB4 to alternate function mode ("10")

    GPIOB->AFRL &= ~(0xF << (FAN_PWM_PIN * 4)); // Clear the 4 AFRL (Alternate Function Register Low) bits for PB4
    GPIOB->AFRL |=  (FAN_PWM_AF << (FAN_PWM_PIN * 4));  // Use AF2 (PWM) for PB4


    // Enable TIM3 peripheral clock
    *(RCC_APB1ENR) |= (1 << TIM3_EN);

    TIM3->PSC = 0;
    TIM3->ARR = FAN_PWM_ARR;

    // Channel 1 as PWM mode 1: output is active while CNT < CCR1.
    // CCMR1 = Capture/Compare Mode Register 1
    TIM3->CCMR1 &= ~(0x7 << 4);     // Clear OC1M (Output Compare 1 Mode) (bits 4-6)
    TIM3->CCMR1 |=  (0x6 << 4);     // Set OC1M to PWM mode 1 ("110") - active-high while CNT < CCR1
    TIM3->CCMR1 |=  (1 << 3);       // Set OC1PE (Output Compare 1 Preload Enable)

    // CCR1 = Capture/Compare Register 1
    TIM3->CCR1 = 0;                 // Init at 0% duty cycle (off)

    // CCER = Capture/Compare Enable Register
    TIM3->CCER |= (1 << 0);         // Set CC1E to enable channel 1 output to PB4

    // CR1 = Control Register 1
    // EGR = Event Generation Register
    TIM3->CR1 |= (1 << 7);          // Set ARPE (Auto-Reload Preload Enable)
    TIM3->EGR |= (1 << 0);          // Set UG (Update Generation) to force an update to load PSC/ARR/CCR1
    TIM3->CR1 |= (1 << 0);          // Set CEN (Counter Enable)
}

void fan_set_speed(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    TIM3->CCR1 = (FAN_PWM_PERIOD * percent) / 100;
}

void fan_tach_init(void)
{
    // Enable GPIOA peripheral clock
    *(RCC_AHB1ENR) |= (1 << GPIOAEN);

    GPIOA->MODER &= ~(0x3 << (TACH_PIN * 2));               // Clear PA0 MODER bits
    GPIOA->MODER |=  (ALTERNATE_FUNCTION << (TACH_PIN * 2)); // Set PA0 to alternate function mode ("10")

    GPIOA->AFRL &= ~(0xF << (TACH_PIN * 4));                // Clear the 4 AFRL (Alternate Function Register Low) bits for PA0
    GPIOA->AFRL |=  (TACH_AF << (TACH_PIN * 4));            // Use AF1 (TIM2_CH1) for PA0

    GPIOA->PUPDR &= ~(0x3 << (TACH_PIN * 2));               // Clear PA0 PUPDR (Pull-Up/Pull-Down Resistor) bits
    GPIOA->PUPDR |=  (0x1 << (TACH_PIN * 2));               // Set to pull-up ("01")

    
    // Enable TIM2 peripheral clock
    *(RCC_APB1ENR) |= (1 << TIM2_EN);

    TIM2->PSC = TACH_PRESCALE;  // 1 MHz tick (1 us per count)
    TIM2->ARR = 0xFFFFFFFF;     // Use full 32-bit range

    // Channel 1 as input: map the capture to input TI1, with a noise filter
    // CCMR1 = Capture/Compare Mode Register 1
    TIM2->CCMR1 &= ~(0xFF << 0);     // Clear CC1S (Capture/Compare 1 Selection), IC1PSC (Input Capture 1 Prescaler), and IC1F (Input Capture 1 Filter)
    TIM2->CCMR1 |=  (0x1 << 0);      // Set CC1S to "01" to map IC1 to input TI1
    TIM2->CCMR1 |=  (0x3 << 4);      // Set IC1F to "0011" to enable an input filter (debounce)

    // CCER = Capture/Compare Enable Register
    TIM2->CCER &= ~((1 << 1) | (1 << 3));   // Clear CC1P and CC1NP (Capture/Compare 1 Polarity) to capture on the rising edge
    TIM2->CCER |=  (1 << 0);                // Set CC1E (Capture/Compare 1 Enable) to enable capture

    // Reset slave mode: a TI1 rising edge captures CNT into CCR1, then resets
    // CNT to 0, so CCR1 holds the period of the most recent pulse.
    // SMCR = Slave Mode Control Register
    TIM2->SMCR &= ~((0x7 << 4) | (0x7 << 0));   // Clear TS (Trigger Selection) and SMS (Slave Mode Selection)
    TIM2->SMCR |=  (0x5 << 4);       // Set TS to "101" (trigger source = TI1FP1)
    TIM2->SMCR |=  (0x4 << 0);       // Set SMS to "100" (reset mode)

    // EGR = Event Generation Register
    // CR1 = Control Register 1
    TIM2->EGR |= (1 << 0);           // Set UG (Update Generation) to force an update to load PSC/ARR
    TIM2->CR1 |= (1 << 0);           // Set CEN (Counter Enable)
}

uint32_t fan_get_rpm(void)
{
    // If the count is greater than the tick rate it means it has been
    // been more than one second since the last tach pulse (half-revolution,)
    // so just return 0 RPM.
    if (TIM2->CNT > TACH_TICK_HZ)
    {
        return 0;
    }

    uint32_t period = TIM2->CCR1;    // microseconds between the last two edges

    if (period == 0)
    {
        return 0;                    // no pulse captured yet (avoid divide-by-zero)
    }

    return TACH_RPM_FACTOR * TACH_TICK_HZ / period;
}
