# ELS 7e2 → Black STM32F407ZET6 (CubeIDE/CubeMX)

Це **каркас** для переносу `7e2` на плату **MCUDev Black STM32F407ZET6**.

Що вже є “з коробки”:
- **UART лог (USART1 PA9/PA10)**: друк статусу та результатів I2C scan
- **LCD 2004 I2C (PCF8574) через I2C2 PB10/PB11**: вивід `ELS F407` + знайдена I2C адреса
- **Spindle encoder**: TIM4 encoder mode (PD12/PD13) — лічильник читається і логиться
- **Handwheel encoder**: TIM3 encoder mode (PC6/PC7) — лічильник читається і логиться
- **STEP генерація**: TIM1 PWM CH1/CH2 (PE9/PE11) — тестова генерація 1 кГц на Z та 500 Гц на X

## Важливо про I2C рівні
Якщо LCD/PCF8574 живиться від **5V** і має підтяжки SDA/SCL до 5V — потрібен **I2C level shifter (BSS138)** або підтяжки мають бути **до 3.3V**.

## Як запустити
1) Відкрий `ELS_F407_BLACK.ioc` у STM32CubeIDE (або STM32CubeMX).
2) Натисни **Generate Code**.
3) Відкрий проект у CubeIDE (якщо не відкрився автоматично), збери і проший через **ST‑Link (SWD)**.
4) У UART (115200 8N1) побачиш:
   - результати I2C scan (типово `0x27` або `0x3F`)
   - значення лічильників TIM3/TIM4

## Де код
- `Core/Src/app_main.c` / `Core/Inc/app_main.h`: `app_init()` + `app_loop()`
- `Core/Src/bsp_uart.c` / `Core/Inc/bsp_uart.h`: друк у UART
- `Core/Src/bsp_i2c.c` / `Core/Inc/bsp_i2c.h`: I2C scan
- `Core/Src/ui_lcd_pcf8574.c` / `Core/Inc/ui_lcd_pcf8574.h`: LCD 2004 I2C
- `Core/Src/bsp_encoder.c` / `Core/Inc/bsp_encoder.h`: читання енкодерів (TIM3/TIM4)
- `Core/Src/bsp_stepgen.c` / `Core/Inc/bsp_stepgen.h`: STEP генерація (TIM1 CH1/CH2)

## Піни (коротко)
Детально див. `STM32F407_PORT.md` у корені репозиторію.
