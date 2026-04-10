#include "bsp_uart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

// Очікуємо, що CubeMX згенерує huart1 (USART1).
extern UART_HandleTypeDef huart1;

void bsp_uart_init(void) {}

void bsp_uart_write(const char *s) {
  if (!s) return;
  (void)HAL_UART_Transmit(&huart1, (uint8_t *)s, (uint16_t)strlen(s), 100);
}

void bsp_uart_printf(const char *fmt, ...) {
  char buf[256];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (n <= 0) return;
  buf[sizeof(buf) - 1] = '\0';
  bsp_uart_write(buf);
}

