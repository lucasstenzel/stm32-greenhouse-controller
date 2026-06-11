/*
 * @file main.c
 * @author Lucas Stenzel
 * @date June 1, 2026
 * 
 * Program ***
 */

#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "delay.h"
#include "fan.h"
#include "gpio.h"
#include "greenhouse_fsm.h"
#include "lcd.h"
#include "relays.h"
#include "sensors.h"
#include "timebase.h"

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define SCB_CPACR   (volatile uint32_t*) 0xE000ED88  // Coprocessor Access Control Register for float math

// Scheduler cadences (non-blocking, gated on millis())
#define CONTROL_PERIOD_MS	1000U   // run the main loop at 1Hz
#define DISPLAY_PERIOD_MS	200U   // refresh the LCD at ~5 Hz

// Force a fresh air exchange (FAE) if once hasn't happened in this long
#define FAE_BACKUP_MS	(10U * 60U * 1000U)  // 10 min
// Once the FAE starts, run the fan for this long
#define FAE_BACKUP_DURATION_MS		(60U * 1000U)        // 1 min

// Forward declaration of helper functions
static void fpu_init(void);
static Event derive_event(float humidity_pct, uint16_t co2_ppm);
static void update_display(float humidity, uint32_t co2);
static void error_display(void);

int main() {
	// Initialize hardware
	fpu_init();
	fan_init();
	//fan_tach_init();
	lcd_init();
	relay_init();
	sensors_init();	// This calls I2C init
	timebase_init();

	State state = STATE_IDLE;
	Event input_event;

	// Latest sensor readings
	float humidity = 0.0f;
	uint16_t co2 = 0;
	float temp_f = 0.0f;
	uint16_t status = 0;

	// Scheduler timestamps
	uint32_t last_control = millis();
	uint32_t last_display = millis();
	uint32_t last_FAE = millis();

	// FAE tracking
	bool ventilating = false;
	uint32_t FAE_start = 0;

	// main loop
	while (1) {
		uint32_t now = millis();

		// Run the control loop at 1Hz 
		if ((uint32_t)(now - last_control) >= CONTROL_PERIOD_MS) {
			last_control = now;

			if (sensors_read(&co2, &temp_f, &humidity, &status)) {
				input_event = derive_event(humidity, co2);

				// Check that the fan has been turned on recently
				if (!ventilating && (uint32_t)(now - last_FAE) >= FAE_BACKUP_MS) {
					ventilating = true;
					FAE_start = now;
				}
				if (ventilating) {
					if ((uint32_t)(now - FAE_start) >= FAE_BACKUP_DURATION_MS) {
						ventilating = false;  // FAE done; hand control back to the sensors
					} else {
						input_event = EVENT_CO2_HIGH;
					}
				}

				state = update_fsm(state, input_event);

				if (state == STATE_VENTILATING) {
					last_FAE = now;  // reset the failsafe window while venting
				}
			}
		}

		// Run the display loop at 5Hz
		if ((uint32_t)(now - last_display) >= DISPLAY_PERIOD_MS) {
			last_display = now;
			update_display(humidity, co2);
		}
	}

	error_display();

	return 0;
}

static void fpu_init() {
	// Enable full access to CP10 and CP11 for the FPU
	*SCB_CPACR |= (0xF << 20); // Set bits 20-23 to enable CP10 and CP11
	__asm volatile ("dsb");  // Wait for the CPACR write to complete
	__asm volatile ("isb");  // Flush the pipeline before any FPU instruction
}

/**
 * Determine the current Event from the relative humidity and CO2 readings.
 * Ventilation is prioritized over humidification.
 */
static Event derive_event(float humidity_pct, uint16_t co2_ppm) {
	/**
	 * Set the humidity target 80% towards the high end of the acceptable range.
	 * The humidifier will run from the low end all the way up to here in order
	 * to provide a solid period of runtime.
	 */
	const float humidity_target =
		(ACTIVE_PROFILE.humidity_low + ACTIVE_PROFILE.humidity_high) * 0.8f;

	/**
	 * High CO2 is the top priority
	 */
	if (co2_ppm >= ACTIVE_PROFILE.co2_high)				return EVENT_CO2_HIGH;

	/**
	 * Now ensure humidity is within acceptable ranges
	 */
	if (humidity_pct > ACTIVE_PROFILE.humidity_high)	return EVENT_HUMIDITY_HIGH;
	if (humidity_pct < ACTIVE_PROFILE.humidity_low)  	return EVENT_HUMIDITY_LOW;

	/**
	 * At this point, CO2 and humidity are acceptable - but might not be ideal.
	 * Let the system keep running if CO2 hasn't reached ideal levels.
	 */
	if (co2_ppm > ACTIVE_PROFILE.co2_target)         	return EVENT_NONE;

	/**
	 * Again, let the system keep running if humidity hasn't reached ideal
	 * levels either.
	 */
	if (humidity_pct < humidity_target)              	return EVENT_NONE;

	/**
	 * At this point one of the measurements should be perfect, and
	 * the other should at least be within the acceptable range.
	 */
	return EVENT_ALL_GOOD;
}

static void update_display(float humidity, uint32_t co2) {
	lcd_clear();
	lcd_home();
	lcd_print_string("Humidity: ");
	lcd_print_float(humidity, 2);
	lcd_print_string("%");

	lcd_set_position(1, 0);
	lcd_print_string("CO2: ");
	lcd_print_num(co2, 0);
	lcd_print_string("ppm");
}

static void error_display() {
	lcd_clear();
	lcd_home();
	lcd_print_string("System Error -");
	lcd_set_position(1, 0);
	lcd_print_string("Reset Required");
}