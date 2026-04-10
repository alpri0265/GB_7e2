#pragma once

#include <stdint.h>

typedef struct {
  uint8_t addr_7bit;
  uint8_t backlight; // 0/1
} lcd_pcf8574_t;

void lcd_pcf8574_init(lcd_pcf8574_t *lcd, uint8_t addr_7bit);
void lcd_pcf8574_clear(lcd_pcf8574_t *lcd);
void lcd_pcf8574_home(lcd_pcf8574_t *lcd);
void lcd_pcf8574_set_cursor(lcd_pcf8574_t *lcd, uint8_t col, uint8_t row);
void lcd_pcf8574_print(lcd_pcf8574_t *lcd, const char *s);
void lcd_pcf8574_backlight(lcd_pcf8574_t *lcd, uint8_t on);

