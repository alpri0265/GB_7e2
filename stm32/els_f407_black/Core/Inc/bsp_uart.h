#pragma once

#include <stddef.h>

void bsp_uart_init(void);
void bsp_uart_write(const char *s);
void bsp_uart_printf(const char *fmt, ...);

