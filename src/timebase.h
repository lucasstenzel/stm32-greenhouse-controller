/*
 * @file timebase.h
 * @author Lucas Stenzel
 *
 * @brief Free-running millisecond time base built on TIM6 (basic timer.)
 *        TIM6 is configured to raise an update interrupt every 1 ms. The ISR
 *        increments a software counter, extending the 16-bit timer into a
 *        32-bit millisecond clock (the prescaler + ARR + software-counter
 *        pattern.) This gives a non-blocking time reference (e.g. for scheduling the
 *        control loop or long timeouts)
 */

#ifndef TIMEBASE_H
#define TIMEBASE_H

#include <stdint.h>

/**
 * @brief Configure TIM6 for a 1ms update interrupt and start the counter.
 */
void timebase_init(void);

/**
 * @brief Milliseconds elapsed since timebase_init().
 * @note  Wraps after ~49.7 days. As long as the computed elapsed time is cast to uint32_t,
 *        the calculation will be correct even across a single wrap.
 *        E.g.  (uint32_t)(millis() - start) >= timeout
 */
uint32_t millis(void);

#endif // TIMEBASE_H