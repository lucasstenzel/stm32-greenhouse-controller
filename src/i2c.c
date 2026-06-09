#include "gpio.h"
#include "i2c.h"

// I2C1 peripheral register block (APB1)
typedef struct {
    uint32_t CR1;
    uint32_t CR2;
    uint32_t OAR1;
    uint32_t OAR2;
    uint32_t DR;
    uint32_t SR1;
    uint32_t SR2;
    uint32_t CCR;
    uint32_t TRISE;
    uint32_t FLTR;
} I2C_T;

// Struct pointers
static volatile GPIO*  GPIOB = (GPIO*)0x40020400;
static volatile I2C_T* I2C1  = (I2C_T*)0x40005400;

#define I2C_SCL_PIN 8    // PB8
#define I2C_SDA_PIN 9    // PB9
#define I2C_AF      4    // AF4 = I2C1

#define I2C1EN 21        // RCC_APB1ENR bit for I2C1

// CR1 bits
#define I2C_CR1_PE    (1 << 0)
#define I2C_CR1_START (1 << 8)
#define I2C_CR1_STOP  (1 << 9)
#define I2C_CR1_ACK   (1 << 10)
#define I2C_CR1_SWRST (1 << 15)
// SR1 bits
#define I2C_SR1_SB    (1 << 0)    // start bit sent
#define I2C_SR1_ADDR  (1 << 1)    // address sent / matched
#define I2C_SR1_BTF   (1 << 2)    // byte transfer finished
#define I2C_SR1_RXNE  (1 << 6)    // receive buffer not empty
#define I2C_SR1_TXE   (1 << 7)    // transmit buffer empty
#define I2C_SR1_AF    (1 << 10)   // acknowledge failure (NACK)
// SR2 bits
#define I2C_SR2_BUSY  (1 << 1)

// Loop count before a blocking wait gives up. For the 100kHz I2C clock,
// this should only happen on an unresponsive bus.
#define I2C_TIMEOUT 100000

void i2c_init(void)
{
    // Enable GPIOB peripheral clock
    *(RCC_AHB1ENR) |= (1 << GPIOBEN);

    GPIOB->MODER &= ~((0x3 << (I2C_SCL_PIN * 2)) | (0x3 << (I2C_SDA_PIN * 2))); // Clear the two MODER bits for PB8 and PB9
    GPIOB->MODER |=  ((ALTERNATE_FUNCTION << (I2C_SCL_PIN * 2)) |               // Set PB8/PB9 to alternate function mode ("10")
                      (ALTERNATE_FUNCTION << (I2C_SDA_PIN * 2)));

    // Open-drain is mandatory for I2C (external pull-up resistor where connected devices pull it low)
    GPIOB->OTYPER |= (1 << I2C_SCL_PIN) | (1 << I2C_SDA_PIN);

    // Adafruit 6478 STEMMA QT breakout already has pull-up resistors, but enable the internal ones for backup
    GPIOB->PUPDR &= ~((0x3 << (I2C_SCL_PIN * 2)) | (0x3 << (I2C_SDA_PIN * 2))); // Clear the two PUPDR bits for PB8 and PB9
    GPIOB->PUPDR |=  ((0x1 << (I2C_SCL_PIN * 2)) | (0x1 << (I2C_SDA_PIN * 2))); // Set PB8/PB9 to pull-up

    // PB8 and PB9 are both in AFRH (pins 8-15); nibble index = pin - 8
    GPIOB->AFRH &= ~((0xF << ((I2C_SCL_PIN - 8) * 4)) | (0xF << ((I2C_SDA_PIN - 8) * 4))); // Clear the 4 AFRH (Alternate Function Register High) bits for PB8/PB9
    GPIOB->AFRH |=  ((I2C_AF << ((I2C_SCL_PIN - 8) * 4)) | (I2C_AF << ((I2C_SDA_PIN - 8) * 4))); // Use AF4 (I2C) for PB8/PB9

    // Enable I2C1 peripheral clock
    // AHB1 is at 16MHz, but APB1 is at 100kHz
    *(RCC_APB1ENR) |= (1 << I2C1EN);

    // Reset the peripheral to a known state before configuring
    I2C1->CR1 |=  I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // Set up the standard 100kHz bus speed
    I2C1->CR2 = 16; // FREQ = APB1 clock frequency, in MHz
    I2C1->CCR = 80; // Clock Control Register = Fpclk1 / (2 * Fscl) = 16 MHz / (2 * 100kHz) = 80
    I2C1->TRISE = 17; // Max SCL rise time = Fpclk1(MHz) + 1 = 17

    I2C1->CR1 |= I2C_CR1_PE;   // Finally, enable the peripheral
}

// Wait until the masked bits in *reg are set, or timeout
static bool wait_set(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = I2C_TIMEOUT;
    while (!(*reg & mask))
    {
        if (--timeout == 0)
        {
            return false;
        }
    }
    return true;
}

// Wait until the BUSY flag is cleared, or timeout
static bool wait_bus_idle(void)
{
    uint32_t timeout = I2C_TIMEOUT;
    while (I2C1->SR2 & I2C_SR2_BUSY)
    {
        if (--timeout == 0)
        {
            return false;
        }
    }
    return true;
}

bool i2c_write(uint8_t addr, const uint8_t *data, uint32_t len)
{
    if (!wait_bus_idle()) return false;

    // START
    I2C1->CR1 |= I2C_CR1_START;
    if (!wait_set(&I2C1->SR1, I2C_SR1_SB)) return false;

    // Address + write direction (LSB = 0)
    I2C1->DR = (uint32_t)(addr << 1);
    if (!wait_set(&I2C1->SR1, I2C_SR1_ADDR))
    {
        I2C1->CR1 |= I2C_CR1_STOP;   // NACK / no device
        return false;
    }

    (void)I2C1->SR1;
    (void)I2C1->SR2;    // clear ADDR

    // Data bytes
    for (uint32_t i = 0; i < len; i++)
    {
        if (!wait_set(&I2C1->SR1, I2C_SR1_TXE))
        {
            I2C1->CR1 |= I2C_CR1_STOP;
            return false;
        }
        I2C1->DR = data[i];
    }

    if (!wait_set(&I2C1->SR1, I2C_SR1_BTF))
    {
        I2C1->CR1 |= I2C_CR1_STOP;
        return false;
    }

    I2C1->CR1 |= I2C_CR1_STOP;
    return true;
}

bool i2c_read(uint8_t addr, uint8_t *data, uint32_t len)
{
    if (len == 0) return true;
    if (!wait_bus_idle()) return false;

    I2C1->CR1 |= I2C_CR1_ACK;        // ACK received bytes

    // START
    I2C1->CR1 |= I2C_CR1_START;
    if (!wait_set(&I2C1->SR1, I2C_SR1_SB)) return false;

    // Address + read direction (LSB = 1)
    I2C1->DR = (uint32_t)((addr << 1) | 1);
    if (!wait_set(&I2C1->SR1, I2C_SR1_ADDR))
    {
        I2C1->CR1 |= I2C_CR1_STOP;
        return false;
    }

    if (len == 1)
    {
        I2C1->CR1 &= ~I2C_CR1_ACK;   // NACK the single byte
        (void)I2C1->SR1;
        (void)I2C1->SR2;             // clear ADDR
        I2C1->CR1 |= I2C_CR1_STOP;   // program STOP before reading

        if (!wait_set(&I2C1->SR1, I2C_SR1_RXNE)) return false;
        data[0] = (uint8_t)I2C1->DR;
        return true;
    }

    // len >= 3 (Sensirion word + CRC). Read with ACK until 3 bytes remain
    (void)I2C1->SR1;
    (void)I2C1->SR2;                 // clear ADDR

    uint32_t i = 0;
    while ((len - i) > 3)
    {
        if (!wait_set(&I2C1->SR1, I2C_SR1_RXNE)) return false;
        data[i++] = (uint8_t)I2C1->DR;
    }

    // Three bytes left: use BTF to sequence the final ACK/STOP correctly so the
    // last byte is NACKed (per the ST master-receiver procedure)
    if (!wait_set(&I2C1->SR1, I2C_SR1_BTF)) return false;
    I2C1->CR1 &= ~I2C_CR1_ACK;       // NACK the byte still incoming (the last)
    data[i++] = (uint8_t)I2C1->DR;   // read byte N-2

    if (!wait_set(&I2C1->SR1, I2C_SR1_BTF)) return false;
    I2C1->CR1 |= I2C_CR1_STOP;       // STOP after the final byte
    data[i++] = (uint8_t)I2C1->DR;   // read byte N-1

    if (!wait_set(&I2C1->SR1, I2C_SR1_RXNE)) return false;
    data[i++] = (uint8_t)I2C1->DR;   // read byte N

    return true;
}
