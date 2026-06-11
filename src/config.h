/*
 * @file config.h
 * @author Lucas Stenzel
 *
 * @brief Edit this file to configure the desired humidity and CO2 setpoints for your
 *       greenhouse. The controller will use these setpoints to determine when to turn on the
 *       humidifier and fans.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ====== Select the profile to use here ======
#define ACTIVE_PROFILE CUSTOM

// Predefined profiles
typedef struct {
    float humidity_low;         // Lower limit for humidity, percent (0-100): below this -> humidify
    float humidity_high;        // Upper limit for humidity, percent (0-100): above this -> ventilate
    uint16_t co2_high;          // Upper limit for CO2 concentration in ppm: at/above this -> ventilate
    uint16_t co2_target;        // Ideal CO2 concentration in ppm: at/below this -> stop ventilating
} SpeciesProfile;

static const SpeciesProfile ENOKI = {
    .humidity_low = 85.0f,
    .humidity_high = 90.0f,
    .co2_high = 2000,
    .co2_target = 1200
};

static const SpeciesProfile LIONS_MANE = {
    .humidity_low = 85.0f,
    .humidity_high = 95.0f,
    .co2_high = 1100,
    .co2_target = 800
};

static const SpeciesProfile OYSTER = {
    .humidity_low = 80.0f,
    .humidity_high = 90.0f,
    .co2_high = 600,
    .co2_target = 400
};

static const SpeciesProfile REISHI = {
    .humidity_low = 85.0f,
    .humidity_high = 95.0f,
    .co2_high = 1000,
    .co2_target = 800
};

static const SpeciesProfile SHIITAKE = {
    .humidity_low = 80.0f,
    .humidity_high = 90.0f,
    .co2_high = 1500,
    .co2_target = 1000
};

static const SpeciesProfile CUSTOM = {
    .humidity_low = 85.0f,
    .humidity_high = 95.0f,
    .co2_high = 600,
    .co2_target = 450
};

#endif // CONFIG_H