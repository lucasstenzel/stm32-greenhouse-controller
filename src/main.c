/*
 * @file main.c
 * @author Lucas Stenzel
 * @date June 1, 2026
 * 
 * Program ***
 */

#include <stdbool.h>
#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>

#include "delay.h"
#include "gpio.h"
#include "greenhouse_fsm.h"
#include "lcd.h"
#include "relays.h"
#include "sensors.h"

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define SCB_CPACR   (volatile uint32_t*) 0xE000ED88  // Coprocessor Access Control Register for float math


// Pointers to the GPIO structures
static volatile GPIO* GPIOB = (GPIO*)0x40020400; // GPIO port B base address, which is where the LEDs are connected

// Forward declaration of helper functions
static void fpu_init(void);
static void update_display(float humidity, uint32_t co2);
static void error_display(void);

int main() {
	// Initialize hardware
	fpu_init();
	lcd_init();




	update_display(50.2468f, 800);

	// main loop
	while (1) {
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

static void led_init() {
	*RCC_AHB1ENR |= (1 << GPIOBEN); // Enable the GPIOB (clock) in RCC_AHB1ENR

    /*
    GPIOB_MODER is a pointer we've already defined, directly to the MODER register of GPIOB. * is used to access it.
    GPIOB is another pointer we've already defined, but it's a struct pointer. Here, the -> operator can be used to access the MODER member of the struct.

    Personally, I think GPIOB->MODER is cleaner. Not sure why extra effort was done to define GPIOB_MODER as a separate pointer.
    */

    GPIOB->MODER &= ~0xFF3FFC00;   // Clear pins 5-10 and 12-15 (LED0-LED5 and LED6-LED9)
    GPIOB->MODER |=  0x55155400;   // Set those pins to output mode ("01")
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