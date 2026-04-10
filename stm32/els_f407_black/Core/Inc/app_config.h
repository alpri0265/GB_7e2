#pragma once

// Мінімальний набір параметрів для тестів/логів.
// Під час портування сюди переноситься блок "ПАРАМЕТРЫ ЖЕЛЕЗА" зі скетча.

// Ліній енкодера на 1 оберт (як у 7e2.ino: ENC_LINE_PER_REV)
#define APP_SPINDLE_ENC_LINE_PER_REV 2000

// Для квадратури x4 можна рахувати так:
// ticks_per_rev = line_per_rev * 4
#define APP_SPINDLE_ENC_TICKS_PER_REV (APP_SPINDLE_ENC_LINE_PER_REV * 4U)

// I2C адреса PCF8574 (типово 0x27 або 0x3F). Якщо не знаєш — дивись I2C scan в UART.
#define APP_LCD_PCF8574_ADDR_7BIT 0x27

