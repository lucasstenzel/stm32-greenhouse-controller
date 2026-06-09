/*
 * @file sensors.h
 * @author Lucas Stenzel
 *
 * @brief Driver for the Adafruit 6478 STEMMA QT breakout board, featuring Sensiron
 *        STCC4 (CO2) and SHT41 (temperature/humidity) sensors.
 *        The SHT41 is wired to the STCC4's secondary I2C port, so the host only
 *        talks to the STCC4 (0x64) on the main bus: a single read returns all three
 *        measurements from the two sensors, and the STCC4 applies RH/T compensation
 *        internally. Built on the shared I2C1 driver (see i2c.h); each 16-bit word is
 *        validated with a CRC-8.
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize the I2C bus and start the STCC4 continuous measurement.
 *        Takes ~20s to fully initialize, during which time the CO2 sensor
 *        will only return 390ppm. After, new readings are available every ~1s.
 * @return true on success
 */
bool sensors_init(void);

/**
 * @brief Read the latest CO2, temperature, and humidity from the STCC4
 * @param co2_ppm       Output: CO2 concentration in ppm
 * @param temperature_f Output: temperature in degrees Fahrenheit
 * @param humidity_pct  Output: relative humidity, in percent (0-100)
 * @param status        Output: raw STCC4 status word (diagnostics)
 * @return true on success, false on bus error, CRC mismatch, or no sample ready
 */
bool sensors_read(uint16_t *co2_ppm, float *temperature_f, float *humidity_pct,
                  uint16_t *status);

#endif // SENSORS_H