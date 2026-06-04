/*
 * @file lcd.h
 * @author Lucas Stenzel
 * @date June 2, 2026
 * 
 * @brief ST7066U LCD display driver header file
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/**
 * ST7066U -> STM32F446RE pin connections; 8-bit interface
 * DB0 -> PA4
 * DB1 -> PA5
 * DB2 -> PA6
 * DB3 -> PA7
 * DB4 -> PA8
 * DB5 -> PA9
 * DB6 -> PA10
 * DB7 -> PA11 (doubles as busy flag)
 * 
 * RS -> PC8 
 * R/W -> PC9
 * E -> PC10
 */

 #define DB0 4
 #define DB1 5
 #define DB2 6
 #define DB3 7
 #define DB4 8
 #define DB5 9
 #define DB6 10
 #define DB7 11
 #define RS  8   
 #define RW  9
 #define E   10

 #define LCD_WIDTH 40
 #define LCD_ADDRESS_OFFSET 0x40
 #define LCD_INSTR_CLEAR 0x01
 #define LCD_INSTR_HOME 0x02
 #define LCD_INSTR_ENTRY_MODE 0x06  // Increment (cursor moves right,) no display shift
 #define LCD_INSTR_DISPLAY_ON 0x0C    // Display on, cursor off, no cursor blinking
 #define LCD_INSTR_FUNCTION_SET 0x38  // 8-bit bus, 2-line display, 5x8 dots format display mode

void lcd_init(void);
void lcd_clear(void);
void lcd_home(void);
void lcd_set_position(uint8_t row, uint8_t col);
void lcd_print_string(char *str_ptr);
void lcd_print_num(uint32_t num, uint8_t width);
void lcd_print_float(float num, uint8_t decimal_places);

#endif // LCD_H