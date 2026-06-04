/*
 * @file fan.h
 * @author Lucas Stenzel
 *
 * @brief PWM fan driver. Generates a 25 kHz PWM signal on
 *        PB4 (TIM3_CH1, AF2) to drive the fan. Power must be supplied
 *        separately (see relays.h) since the fans require 12V.
 */

#ifndef FAN_H
#define FAN_H

#include <stdint.h>

/**
 * @brief Configure TIM3_CH1 on PB4 for 25 kHz PWM output, starting at 0% (off)
 */
void fan_init(void);

/**
 * @brief Set the fan speed as a duty cycle percentage, 0 (off) to 100 (full)
 */
void fan_set_speed(uint8_t percent);

/**
 * @brief Configure TIM2_CH1 on PA0 as a tachometer input (rising-edge period
 *        capture) for measuring actual fan speed
 */
void fan_tach_init(void);

/**
 * @brief Read the measured fan speed in RPM (0 if stopped/stalled)
 */
uint32_t fan_get_rpm(void);

#endif // FAN_H
