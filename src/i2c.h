/*
 * @file i2c.h
 * @author Lucas Stenzel
 *
 * @brief I2C driver to set up and interface with I2C1 on PB8/PB9 (Alternate Function 4.)
 *        Uses the 100kHz standard bus speed to interface with the SHT41 and STCC4 sensors.
 *        Addresses are 7-bit, and the read/write direction bit is added internally.
 */

#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize GPIOB (PB8 = SCL, PB9 = SDA, AF4, open-drain) and the I2C1
 *        peripheral for 100kHz standard-mode operation
 */
void i2c_init(void);

/**
 * @brief Write a buffer to a 7-bit slave address (START, data, STOP)
 * @return true on success, false on timeout / NACK
 */
bool i2c_write(uint8_t addr, const uint8_t *data, uint32_t len);

/**
 * @brief Read a buffer from a 7-bit slave address (START, data, STOP)
 * @note  Reads from the two Sensiron sensors should be multiples of 3; a 2-byte word plus
 *        its CRC
 * @return true on success, false on timeout / NACK
 */
bool i2c_read(uint8_t addr, uint8_t *data, uint32_t len);

#endif // I2C_H
