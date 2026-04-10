#pragma once

#include <stdint.h>

// Повертає 1 якщо пристрій відповідає, 0 якщо ні
int bsp_i2c_is_ready_7bit(uint8_t addr_7bit);

// Друкує знайдені адреси у UART, повертає кількість знайдених
int bsp_i2c_scan_and_log(void);

