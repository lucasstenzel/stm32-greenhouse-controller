#include "gpio.h"
#include "relays.h"

// Struct pointers
static volatile GPIO* GPIOC = (GPIO*)0x40020800;    // Using PC0 and PC1

#define RELAY_HUMIDIFIER_PIN 0   // PC0
#define RELAY_FAN_PIN        1   // PC1

// Forward declaration of helper functions
static uint8_t get_relay_pin(Device device);

void relay_init(void)
{
    // Enable the GPIOC peripheral clock
    *(RCC_AHB1ENR) |= (1 << GPIOCEN);

    // Preset both pins to the OFF state
    relay_off(DEVICE_HUMIDIFIER);
    relay_off(DEVICE_FAN);

    // Set PC0 and PC1 to output mode ("01"); two MODER bits per pin.
    GPIOC->MODER &= ~((0x3 << (RELAY_HUMIDIFIER_PIN * 2)) | (0x3 << (RELAY_FAN_PIN * 2)));
    GPIOC->MODER |=  ((OUTPUT << (RELAY_HUMIDIFIER_PIN * 2)) | (OUTPUT << (RELAY_FAN_PIN * 2)));
}

void relay_set(Device device, bool on)
{
    uint8_t pin = get_relay_pin(device);

#if RELAY_ACTIVE_LOW
    on = !on;   // Invert if the relay is active-low
#endif

    if (on)
    {
        GPIOC->BSRR = (1 << pin);           // drive pin HIGH
    }
    else
    {
        GPIOC->BSRR = (1 << (pin + 16));    // drive pin LOW (reset half of BSRR)
    }
}

/**
 * @brief Get the GPIO pin number corresponding to a given device
 */
static uint8_t get_relay_pin(Device device)
{
    switch (device)
    {
        case DEVICE_FAN:
            return RELAY_FAN_PIN;
        case DEVICE_HUMIDIFIER:
        default:
            return RELAY_HUMIDIFIER_PIN;
    }
}