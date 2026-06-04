/*
 * @file relays.h
 * @author Lucas Stenzel
 *
 * @brief On/off relay driver. Drives two opto-relay inputs using plain GPIO
 *        outputs:
 *          - PC0 -> humidifier relay
 *          - PC1 -> fan 12 V supply relay (fan speed is controlled separately
 *                   by PWM on TIM3_CH1 once the relay has powered it)
 */

#ifndef RELAYS_H
#define RELAYS_H

#include <stdbool.h>

#define RELAY_ACTIVE_LOW 0  // Change to 1 if the relay module is active-low (inverts the control signal)

typedef enum {
    DEVICE_HUMIDIFIER,   // PC0
    DEVICE_FAN           // PC1
} Device;

/**
 * @brief Initialize the relay control pins (PC0 and PC1) as outputs and set them to the OFF state
 */
void relay_init(void);

/* Drive a relay on/off (handles the active-low inversion internally). */
void relay_set(Device device, bool on);

#endif // RELAYS_H
