

#include "delay.h"
#include "gpio.h"
#include "lcd.h"


// Struct pointers
static volatile GPIO* GPIOA = (GPIO*)0x40020000;
static volatile GPIO* GPIOC = (GPIO*)0x40020800;

// Forward declaration of helper functions
static void lcd_wait_for_ready(void);
static void lcd_write_instr(uint8_t instr);
static void lcd_write_instr_no_wait(uint8_t instr);
static void lcd_write_data(uint8_t data);
static void lcd_write_data_no_wait(uint8_t data);

void lcd_init(void)
{
    // Enable GPIOA and GPIOC peripheral clocks
    *(RCC_AHB1ENR) |= (1 << GPIOAEN) | (1 << GPIOCEN);

    // MODER register has two control bits per pin
    GPIOC->MODER &= ~0x003F0000;    // Clear PC8-PC10 (for RS, RW, E)
    GPIOC->MODER |=  0x00150000;    // Set PC8-PC10 to output mode ("01")

    GPIOA->MODER &= ~0x00FFFF00;    // Clear PA4-PA11 (for DB0-DB7)
    GPIOA->MODER |= 0x00555500;     // Set PA4-PA11 to output mode ("01")

    delay_ms(45);   // Delay at least 40ms

    lcd_write_instr_no_wait(LCD_INSTR_FUNCTION_SET);
    delay_us(40);
    lcd_write_instr_no_wait(LCD_INSTR_FUNCTION_SET);
    delay_us(40);

    lcd_write_instr(LCD_INSTR_DISPLAY_ON);
    lcd_clear();
    lcd_home();
    lcd_write_instr(LCD_INSTR_ENTRY_MODE);
}

void lcd_clear(void)
{
    lcd_write_instr(LCD_INSTR_CLEAR);
    delay_ms(2);
}

void lcd_home(void)
{
    lcd_write_instr(LCD_INSTR_HOME);
    delay_ms(2);
}

void lcd_set_position(uint8_t row, uint8_t col)
{
    if (col >= LCD_WIDTH)
    {
        col = LCD_WIDTH - 1;
    }

    uint8_t pos = col;

    if (row == 1)
    {
        pos += LCD_ADDRESS_OFFSET;
    }

    lcd_write_instr((1 << 7) | pos);    // DB7 must be 1 to set the DDRAM address
}

void lcd_print_string(char *str_ptr)
{
    for (uint8_t i = 0; str_ptr[i] != '\0'; i++)
    {
        lcd_write_data(str_ptr[i]);
    }
}

void lcd_print_num(uint32_t num, uint8_t width)
{
    char buffer[10];
    uint8_t i = 0;

    if (width == 0)
    {
        // Just convert the number to string without padding
        while (num > 0)
        {
            buffer[i++] = (num % 10) + '0'; // Convert last digit to ASCII and store in buffer
            num /= 10; // Remove last digit
        }
    }
    else
    {
        // Convert the number to string with leading zeros if necessary
        while (i < width)
        {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    
    // Write digits to LCD in reverse order
    while (i > 0)
    {
        lcd_write_data(buffer[--i]);
    }
}

void lcd_print_float(float num, uint8_t decimal_places)
{
    if (decimal_places > 4)
    {
        decimal_places = 4; // Limit to 4 decimal places to avoid overflow
    }

    uint32_t integer_part = (uint32_t)num;
    
    float fractional = num - integer_part;
    for (uint8_t i = 0; i < decimal_places; i++)
    {
        fractional *= 10;
    }
    uint32_t fractional_part = (uint32_t)fractional;

    lcd_print_num(integer_part, 0);
    lcd_print_string(".");
    lcd_print_num(fractional_part, decimal_places);
}

static void lcd_write_instr(uint8_t instr)
{
    lcd_wait_for_ready();
    lcd_write_instr_no_wait(instr);
}

static void lcd_write_instr_no_wait(uint8_t instr)
{
    GPIOA->MODER &= ~0x00FFFF00;    // Clear PA4-PA11 (for DB0-DB7)
    GPIOA->MODER |= 0x00555500;     // Set PA4-PA11 to output mode ("01")

    GPIOC->BSRR = (1 << (RS + 16)) | (1 << (RW + 16));  // RS low, RW low

    GPIOA->ODR &= ~0x00000FF0;
    GPIOA->ODR |= (uint32_t)instr << DB0;

    // Toggle E 
    GPIOC->BSRR = (1 << E);
    delay_us(1);
    GPIOC->BSRR = (1 << (E + 16));
    delay_us(1);
}

static void lcd_write_data(uint8_t data)
{
    lcd_wait_for_ready();
    lcd_write_data_no_wait(data);
}

static void lcd_write_data_no_wait(uint8_t data)
{
    GPIOA->MODER &= ~0x00FFFF00;    // Clear PA4-PA11 (for DB0-DB7)
    GPIOA->MODER |= 0x00555500;     // Set PA4-PA11 to output mode ("01")

    GPIOC->BSRR = (1 << (RS)) | (1 << (RW + 16));   // RS high, RW low

    GPIOA->ODR &= ~0x00000FF0;
    GPIOA->ODR |= (uint32_t)data << DB0;

    // Toggle E 
    GPIOC->BSRR = (1 << E);
    delay_us(1);
    GPIOC->BSRR = (1 << (E + 16));
    delay_us(1);
}

static void lcd_wait_for_ready(void)
{
    delay_us(80);
    GPIOA->MODER &= ~0x00FFFF00;    // Set MODER to input mode ("00")
    GPIOC->BSRR = (1 << (RS + 16)) | (1 << RW);     // RS low, RW high

    uint8_t busy_flag;
    do
    {
        // Raise E
        GPIOC->BSRR = (1 << E);
        delay_us(2);

        busy_flag = GPIOA->IDR & (1 << DB7);

        // Lower E
        GPIOC->BSRR = (1 << (E + 16));
        delay_us(2);
    }
    while (busy_flag);
}