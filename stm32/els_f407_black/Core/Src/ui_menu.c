#include "ui_menu.h"

#include <stdio.h>
#include <string.h>

#include "bsp_ui_inputs.h"
#include "bsp_uart.h"

static const char *mode_name(els_mode_t m) {
  switch (m) {
    case ELS_MODE_FEED:
      return "Feed";
    case ELS_MODE_AFEED:
      return "aFeed";
    case ELS_MODE_CONE_L:
      return "Cone L";
    case ELS_MODE_CONE_R:
      return "Cone R";
    case ELS_MODE_THREAD:
      return "Thread";
    case ELS_MODE_SPHERE:
      return "Sphere";
    case ELS_MODE_TACHO:
      return "Tacho";
    case ELS_MODE_RESERVE:
      return "Reserve";
    default:
      return "----";
  }
}

static const char *submode_name(els_submode_t s) {
  switch (s) {
    case ELS_SUBMODE_INT:
      return "Int";
    case ELS_SUBMODE_MAN:
      return "Man";
    case ELS_SUBMODE_EXT:
      return "Ext";
    default:
      return "--";
  }
}

static void lcd_line_pad20(lcd_pcf8574_t *lcd, uint8_t row, const char *s) {
  char buf[21];
  snprintf(buf, sizeof(buf), "%-20.20s", s ? s : "");
  buf[20] = '\0';
  lcd_pcf8574_set_cursor(lcd, 0, row);
  lcd_pcf8574_print(lcd, buf);
}

static bsp_ui_stable_t s_prev;
static uint8_t s_have_prev;
static lcd_pcf8574_t *s_lcd;

void ui_menu_init(lcd_pcf8574_t *lcd) {
  s_lcd = lcd;
  s_have_prev = 0;
  bsp_ui_inputs_init();
}

static void log_edges(const bsp_ui_stable_t *cur) {
  if (!s_have_prev) {
    return;
  }

  if (cur->mode != s_prev.mode) {
    bsp_uart_printf("[UI] mode=%s\r\n", mode_name(cur->mode));
  }
  if (cur->submode != s_prev.submode) {
    bsp_uart_printf("[UI] submode=%s\r\n", submode_name(cur->submode));
  }

  /* Меню-кнопки: активний LOW на PF0..3 (як у Arduino). */
  uint8_t prev_m = (uint8_t)(s_prev.menu_keys & 0x0FU);
  uint8_t cur_m = (uint8_t)(cur->menu_keys & 0x0FU);
  uint8_t pressed = (uint8_t)(prev_m & ~cur_m);
  if (pressed & (1U << 0)) {
    bsp_uart_write("[UI] key DOWN\r\n");
  }
  if (pressed & (1U << 1)) {
    bsp_uart_write("[UI] key UP\r\n");
  }
  if (pressed & (1U << 2)) {
    bsp_uart_write("[UI] key RIGHT\r\n");
  }
  if (pressed & (1U << 3)) {
    bsp_uart_write("[UI] key LEFT\r\n");
  }

  if (cur->select_high != s_prev.select_high && !cur->select_high) {
    bsp_uart_write("[UI] SELECT\r\n");
  }

  /* Джойстик: зміна стану (сирі 4 біти PG0..3). */
  if (cur->joy != s_prev.joy) {
    bsp_uart_printf("[UI] joy=0x%X rapid=%u\r\n", (unsigned)cur->joy, (unsigned)cur->rapid_high);
  }
}

static void draw_screen(const bsp_ui_stable_t *u) {
  if (!s_lcd) {
    return;
  }

  lcd_line_pad20(s_lcd, 0, "ELS F407  Menu");
  lcd_line_pad20(s_lcd, 1, mode_name(u->mode));

  char line[21];
  snprintf(line, sizeof(line), "Sub:%s R:%c", submode_name(u->submode),
           u->rapid_high ? '1' : '0');
  lcd_line_pad20(s_lcd, 2, line);

  snprintf(line, sizeof(line), "Joy:%X Lim:%02X", (unsigned)(u->joy & 0x0FU),
           (unsigned)(u->limits & 0x47U));
  lcd_line_pad20(s_lcd, 3, line);
}

void ui_menu_process(lcd_pcf8574_t *lcd, uint32_t now_ms) {
  (void)lcd;
  bsp_ui_inputs_poll(now_ms);

  bsp_ui_stable_t cur;
  bsp_ui_inputs_get_stable(&cur);

  if (!s_have_prev) {
    s_prev = cur;
    s_have_prev = 1;
    draw_screen(&cur);
    return;
  }

  if (memcmp(&cur, &s_prev, sizeof(cur)) != 0) {
    log_edges(&cur);
    draw_screen(&cur);
    s_prev = cur;
  }
}
