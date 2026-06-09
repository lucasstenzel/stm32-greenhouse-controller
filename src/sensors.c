

#include "delay.h"
#include "i2c.h"
#include "sensors.h"

/**
 * Sensirion CRC-8 (polynomial 0x31, init 0xFF, no reflection, no final XOR).
 * Each 16-bit data word from the sensor is followed by one of these checksums.
 */

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT       0xFF

static uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = CRC8_INIT;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i];

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (uint8_t)((crc << 1) ^ CRC8_POLYNOMIAL);
            }
            else
            {
                crc = (uint8_t)(crc << 1);
            }
        }
    }

    return crc;
}

/**
 * STCC4 (per datasheet rev D1)
 * 
 * The SHT41 is connected to the secondary I2C port on the STCC4 so that
 * you only have to query the STCC4 to get measurements.
 * The STCC4 is at 0x64, and the read_measurement command (0xEC05) returns
 * four 16-bit words, each with a trailing CRC-8:
 *  0 - CO2 concentration, in ppm (no scaling)
 *  1 - temperature (SHT4x encoding)
 *  2 - relative humidity (SHT4x encoding)
 *  3 - sensor status
 * The STCC4 compensates CO2 with the SHT41 internally, so the set_rht_compensation
 * command should not be used.
 */

#define STCC4_ADDR                  0x64
#define STCC4_CMD_START_CONTINUOUS  0x218B  // continuous mode command
#define STCC4_CMD_READ_MEASUREMENT  0xEC05  // read measurement command
#define STCC4_READ_DELAY_MS         1       // read_measurement execution time

// Send a 16-bit command (big-endian) to a Sensirion device
static bool send_command16(uint8_t addr, uint16_t command)
{
    uint8_t buf[2] = { (uint8_t)(command >> 8), (uint8_t)(command & 0xFF) };
    return i2c_write(addr, buf, 2);
}

bool sensors_init(void)
{
    i2c_init();

    return send_command16(STCC4_ADDR, STCC4_CMD_START_CONTINUOUS);
}

bool sensors_read(uint16_t *co2_ppm, float *temperature_f, float *humidity_pct,
                  uint16_t *status)
{
    if (!send_command16(STCC4_ADDR, STCC4_CMD_READ_MEASUREMENT))
    {
        return false;
    }

    delay_ms(STCC4_READ_DELAY_MS);

    // Read all four words (CO2, temperature, humidity, status), each a 2-byte
    // word + trailing CRC, for 12 bytes total
    uint8_t rx[12];
    if (!i2c_read(STCC4_ADDR, rx, sizeof(rx)))   // false if no sample ready (NACK)
    {
        return false;
    }

    // Validate each word's CRC
    if (crc8(&rx[0], 2)  != rx[2]  ||
        crc8(&rx[3], 2)  != rx[5]  ||
        crc8(&rx[6], 2)  != rx[8]  ||
        crc8(&rx[9], 2)  != rx[11])
    {
        return false;
    }

    uint16_t co2_raw     = ((uint16_t)rx[0] << 8) | rx[1];
    uint16_t t_raw       = ((uint16_t)rx[3] << 8) | rx[4];
    uint16_t rh_raw      = ((uint16_t)rx[6] << 8) | rx[7];
    uint16_t status_raw  = ((uint16_t)rx[9] << 8) | rx[10];

    *co2_ppm = co2_raw;   // CO2 is reported directly in ppm

    // SHT4x conversion formula for temperature
    float temperature_c = -45.0f + 175.0f * ((float)t_raw / 65535.0f);
    *temperature_f = (temperature_c * 1.8f) + 32.0f;

    // SHT4x conversion formula for humidity
    float rh = -6.0f + 125.0f * ((float)rh_raw / 65535.0f);
    if (rh < 0.0f)   rh = 0.0f;
    if (rh > 100.0f) rh = 100.0f;   // Clamp rh to 0-100%
    *humidity_pct = rh;

    // Raw STCC4 sensor status bits for diagnostics
    *status = status_raw;

    return true;
}
