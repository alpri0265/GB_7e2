#include "bsp_i2c.h"

#include "bsp_uart.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

int bsp_i2c_is_ready_7bit(uint8_t addr_7bit) {
  return (HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(addr_7bit << 1), 2, 10) == HAL_OK) ? 1 : 0;
}

int bsp_i2c_scan_and_log(void) {
  int found = 0;
  bsp_uart_write("I2C scan...\r\n");
  for (uint8_t addr = 1; addr < 127; addr++) {
    if (bsp_i2c_is_ready_7bit(addr)) {
      found++;
      bsp_uart_printf("Found: 0x%02X\r\n", addr);
    }
  }
  bsp_uart_printf("I2C scan done. Found=%d\r\n", found);
  return found;
}

