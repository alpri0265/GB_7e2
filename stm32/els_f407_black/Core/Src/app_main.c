#include "app_main.h"

#include "app_config.h"
#include "bsp_encoder.h"
#include "bsp_i2c.h"
#include "bsp_stepgen.h"
#include "bsp_uart.h"
#include "ui_lcd_pcf8574.h"

#include "main.h"

static lcd_pcf8574_t g_lcd;
static uint8_t g_lcd_addr = APP_LCD_PCF8574_ADDR_7BIT;

static uint32_t g_last_log_ms = 0;

static void lcd_banner(void) {
  lcd_pcf8574_clear(&g_lcd);
  lcd_pcf8574_set_cursor(&g_lcd, 0, 0);
  lcd_pcf8574_print(&g_lcd, "ELS F407");
  lcd_pcf8574_set_cursor(&g_lcd, 0, 1);
  lcd_pcf8574_print(&g_lcd, "I2C addr:");
  char buf[8];
  snprintf(buf, sizeof(buf), "0x%02X", g_lcd_addr);
  lcd_pcf8574_set_cursor(&g_lcd, 10, 1);
  lcd_pcf8574_print(&g_lcd, buf);
}

static uint8_t autodetect_lcd_addr(void) {
  // Try common addresses quickly
  const uint8_t candidates[] = {APP_LCD_PCF8574_ADDR_7BIT, 0x27, 0x3F, 0x20, 0x38};
  for (size_t i = 0; i < sizeof(candidates); i++) {
    if (bsp_i2c_is_ready_7bit(candidates[i])) return candidates[i];
  }
  // full scan (slow)
  for (uint8_t a = 1; a < 127; a++) {
    if (bsp_i2c_is_ready_7bit(a)) return a;
  }
  return APP_LCD_PCF8574_ADDR_7BIT;
}

void app_init(void) {
  bsp_uart_init();

  bsp_uart_write("\r\nELS F407 bootstrap\r\n");
  bsp_i2c_scan_and_log();

  g_lcd_addr = autodetect_lcd_addr();
  lcd_pcf8574_init(&g_lcd, g_lcd_addr);
  lcd_banner();

  bsp_encoder_init();
  bsp_stepgen_init();

  // Test: start Z at 1kHz, X at 500Hz
  bsp_stepgen_set_dir(AXIS_Z, 1);
  bsp_stepgen_set_dir(AXIS_X, 1);
  bsp_stepgen_set_freq_hz(AXIS_Z, 1000);
  bsp_stepgen_set_freq_hz(AXIS_X, 500);
  bsp_stepgen_start(AXIS_Z);
  bsp_stepgen_start(AXIS_X);

  g_last_log_ms = HAL_GetTick();
}

void app_loop(void) {
  uint32_t now = HAL_GetTick();
  if ((now - g_last_log_ms) < 500) return;
  g_last_log_ms = now;

  int32_t sp = bsp_encoder_get_count(ENCODER_SPINDLE);
  int32_t hw = bsp_encoder_get_count(ENCODER_HANDWHEEL);

  bsp_uart_printf("spindle_cnt=%ld hand_cnt=%ld\r\n", (long)sp, (long)hw);

  // Show counts on LCD (2 lines)
  char line[21];
  snprintf(line, sizeof(line), "SP:%ld", (long)sp);
  lcd_pcf8574_set_cursor(&g_lcd, 0, 2);
  lcd_pcf8574_print(&g_lcd, "                    ");
  lcd_pcf8574_set_cursor(&g_lcd, 0, 2);
  lcd_pcf8574_print(&g_lcd, line);

  snprintf(line, sizeof(line), "HW:%ld", (long)hw);
  lcd_pcf8574_set_cursor(&g_lcd, 0, 3);
  lcd_pcf8574_print(&g_lcd, "                    ");
  lcd_pcf8574_set_cursor(&g_lcd, 0, 3);
  lcd_pcf8574_print(&g_lcd, line);
}

