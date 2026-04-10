#include "ui_lcd_pcf8574.h"

#include <string.h>

#include "main.h"

extern I2C_HandleTypeDef hi2c2;

// PCF8574→LCD типове підключення:
// P0: RS
// P1: RW (звичайно 0)
// P2: E
// P3: Backlight
// P4: D4
// P5: D5
// P6: D6
// P7: D7

#define LCD_PIN_RS 0x01
#define LCD_PIN_RW 0x02
#define LCD_PIN_E  0x04
#define LCD_PIN_BL 0x08

static HAL_StatusTypeDef pcf_write(lcd_pcf8574_t *lcd, uint8_t data) {
  uint8_t out = data | (lcd->backlight ? LCD_PIN_BL : 0);
  return HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)(lcd->addr_7bit << 1), &out, 1, 20);
}

static void pulse_enable(lcd_pcf8574_t *lcd, uint8_t data) {
  // E low → high → low
  (void)pcf_write(lcd, data & ~LCD_PIN_E);
  (void)pcf_write(lcd, data | LCD_PIN_E);
  (void)pcf_write(lcd, data & ~LCD_PIN_E);
}

static void write4bits(lcd_pcf8574_t *lcd, uint8_t nibble_with_ctrl) {
  pulse_enable(lcd, nibble_with_ctrl);
}

static void send(lcd_pcf8574_t *lcd, uint8_t value, uint8_t mode_rs) {
  uint8_t hi = (uint8_t)(value & 0xF0);
  uint8_t lo = (uint8_t)((value << 4) & 0xF0);
  uint8_t ctrl = (uint8_t)(mode_rs ? LCD_PIN_RS : 0);

  write4bits(lcd, (uint8_t)(hi | ctrl));
  write4bits(lcd, (uint8_t)(lo | ctrl));
}

static void cmd(lcd_pcf8574_t *lcd, uint8_t c) {
  send(lcd, c, 0);
  HAL_Delay(2);
}

static void data(lcd_pcf8574_t *lcd, uint8_t d) {
  send(lcd, d, 1);
}

void lcd_pcf8574_backlight(lcd_pcf8574_t *lcd, uint8_t on) {
  if (!lcd) return;
  lcd->backlight = on ? 1 : 0;
  (void)pcf_write(lcd, 0);
}

void lcd_pcf8574_init(lcd_pcf8574_t *lcd, uint8_t addr_7bit) {
  if (!lcd) return;
  lcd->addr_7bit = addr_7bit;
  lcd->backlight = 1;

  HAL_Delay(50);

  // 4-bit init sequence (HD44780 datasheet)
  // send 0x03 3 times, then 0x02
  write4bits(lcd, 0x30);
  HAL_Delay(5);
  write4bits(lcd, 0x30);
  HAL_Delay(5);
  write4bits(lcd, 0x30);
  HAL_Delay(2);
  write4bits(lcd, 0x20);
  HAL_Delay(2);

  // Function set: 4-bit, 2-line, 5x8 dots (для 20x4 теж OK)
  cmd(lcd, 0x28);
  // Display on, cursor off, blink off
  cmd(lcd, 0x0C);
  // Clear display
  cmd(lcd, 0x01);
  HAL_Delay(3);
  // Entry mode set: increment, no shift
  cmd(lcd, 0x06);
}

void lcd_pcf8574_clear(lcd_pcf8574_t *lcd) {
  if (!lcd) return;
  cmd(lcd, 0x01);
  HAL_Delay(3);
}

void lcd_pcf8574_home(lcd_pcf8574_t *lcd) {
  if (!lcd) return;
  cmd(lcd, 0x02);
  HAL_Delay(3);
}

void lcd_pcf8574_set_cursor(lcd_pcf8574_t *lcd, uint8_t col, uint8_t row) {
  static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (!lcd) return;
  if (row > 3) row = 3;
  cmd(lcd, (uint8_t)(0x80 | (col + row_offsets[row])));
}

void lcd_pcf8574_print(lcd_pcf8574_t *lcd, const char *s) {
  if (!lcd || !s) return;
  while (*s) {
    data(lcd, (uint8_t)(*s));
    s++;
  }
}

