#pragma once

#include <stdint.h>

#include "ui_lcd_pcf8574.h"

void ui_menu_init(lcd_pcf8574_t *lcd);
void ui_menu_process(lcd_pcf8574_t *lcd, uint32_t now_ms);
