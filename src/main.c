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

#include "actuator.h"
#include "co2.h"
#include "gpio.h"
#include "greenhouse_fsm.h"
#include "humidity.h"
#include "lcd.h"

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830

// Pointers to the GPIO structures
static volatile GPIO* GPIOB = (GPIO*)0x40020400; // GPIO port B base address, which is where the LEDs are connected

// File-scope helper methods
static void led_init();
static void led_allOn();
static void led_allOff();

int main() {
	
	led_init();

	led_allOn();

	// main loop
	while (1) {
	}
	
	// This will never be reached
	led_allOff();

	return 0;
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

static void led_allOn() {
	GPIOB->BSRR |= (0xF7E0); // Turn on all LEDs by setting the lower half-word of BSRR
}

static void led_allOff() {
	GPIOB->BSRR |= (0xF7E0 << 16); // Turn off all LEDs by setting the upper half-word of BSRR
}