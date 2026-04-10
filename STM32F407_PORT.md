# Порт `7e2` (Arduino Mega) на STM32F407ZET6 (STM32CubeIDE)

Цей проєкт — AVR/Arduino-скетч з “жорстким” реалтаймом: багато прямої роботи з `DDR/PORT/PIN`, таймерами `TCCR/OCR/TIMSK`, та ISR. Нижче — інвентар сигналів, рекомендація по таймерам STM32F407 і стратегія переносу під CubeIDE (HAL/LL).

## 1) Інвентар сигналів (що підключено зараз)

Джерело: `7e2.ino` (макроси GPIO + ISR), `Menu.ino`, `ADC.ino`, `HandCoder.ino`, `Beeper.ino`.

### 1.1 LCD (HD44780 через `LiquidCrystalRus`)

У `7e2.ino`:
- **LCD**: `LiquidCrystalRus lcd(8, 9, 10, 11, 12, 13);`
  - **RS** = D8
  - **E** = D9
  - **D4..D7** = D10..D13
  - RW не використовується (працює як типовий 4-bit write-only режим).

На STM32: краще або перейти на **I2C backpack (PCF8574)**, або залишити паралельний HD44780, але написати драйвер під GPIO (HAL/LL) і вирішити питання **3.3V↔5V**.

### 1.2 Stepper Z/X (STEP/DIR/ENA) + тахо-імпульс

У `7e2.ino` (порт L, є коментарі з Arduino Mega pin):
- **Z_STEP**: `PL0` (Arduino **D49**) — `Motor_Z_*Pulse()`
- **X_STEP**: `PL1` (Arduino **D48**) — `Motor_X_*Pulse()`
- **X_ENA**: `PL3` (Arduino **D46**) — `Motor_X_Enable/Disable()`
- **Z_ENA**: `PL4` (Arduino **D45**) — `Motor_Z_Enable/Disable()`
- **X_DIR**: `PL5` (Arduino **D44**) — `Motor_X_CW/CCW()`
- **Z_DIR**: `PL6` (Arduino **D43**) — `Motor_Z_CW/CCW()`
- **TACHO_OUT**: `PL7` (Arduino **D42**) — `TachoSet/RemovePulse()`

Примітка: кроки реалізовані як toggle (`InvertPulse()`), а “фронт” визначається читанням стану (`Read_Z_State`, `Read_X_State`). На STM32 краще або апаратний PWM/OC toggle, або software-state без читання GPIO.

### 1.3 Енкодер шпинделя (квадратура) + INT0

У `7e2.ino`:
- `Enc_Ch_A = (PIND & (1<<0))` → **PD0** (Arduino **D21** на Mega)
- `Enc_Ch_B = (PIND & (1<<1))` → **PD1** (Arduino **D20** на Mega)
- ISR: `ISR(INT0_vect)` (на Mega INT0 прив’язаний до **D21/PD0**).

### 1.4 Handwheel (ручний енкодер) + вибір осі/масштабу

У `7e2.ino`:
- **Handwheel A/B**: `PD2` (Arduino **D19**), `PD3` (Arduino **D18**); ISR `INT2`.
- **Вибір осі (Z/X/None)**: `Hand_Axis_Read = (PINE & 0x30)` → **PE4/PE5**
- **Масштаб (x1/x10)**: `Hand_Scale_Read = (PINJ & 0x03)` → **PJ0/PJ1**

### 1.5 Ліміти + LED індикація

У `7e2.ino`:
- **Limit buttons in**: `PINA & 0x55` → **PA0, PA2, PA4, PA6** (підтяжки)
- **LED out**:
  - Rear: **PA1**
  - Front: **PA3**
  - Right: **PA5**
  - Left: **PA7**

### 1.6 Меню-кнопки, Select

У `7e2.ino`:
- **Buttons**: `PINF & 0x0F` → **PF0..PF3**
- **Select**: `PINF & 0x10` → **PF4**

### 1.7 Джойстик + Rapid + перемикач підрежиму

У `7e2.ino`:
- **Joystick**: `PINK & 0x0F` → **PK0..PK3**
- **Rapid**: `PINK & 0x10` → **PK4**
- **Submode**: `PINK & 0xE0` → **PK5..PK7**

### 1.8 Перемикач режимів (8 позицій)

У `7e2.ino`:
- **Mode**: `PINC & 0xFF` → **PC0..PC7**

### 1.9 Beeper (зумер)

У `7e2.ino`:
- **Buzzer**: `PH1` (Arduino **D16**) через `PORTH`.

### 1.10 Аналоговий вхід (потенціометр подачі)

У `ADC.ino`:
- `analogRead(A7)` → **A7** (на Mega це **PF7/ADC7**).

### 1.11 Інші сигнали

У `7e2.ino`:
- **D53**: `pinMode(53, OUTPUT)` + `digitalWrite(53, ...)` — “делилка/ознака” (на STM32 буде окремий GPIO).
- `pulseIn()` використовує `pin = 20`, але D20 вже зайнятий енкодером. На STM32 краще прибрати `pulseIn()` і рахувати RPM від таймера-енкодера.
- `DDRG = 0xFF` — порт G виставляється на вихід, але не використовується в логіці (перевірити по схемі).

## 2) Рекомендований мапінг STM32F407 (таймери та канали)

Нижче — мапінг по **функціях**. Конкретні піни під `STM32F407ZET6-MODUL` треба вибрати в CubeMX (він покаже доступні AF на твоїй платі/корпусі).

### 2.1 Шпиндельний енкодер

- **TIMx в Encoder Interface mode** (CH1+CH2) для A/B.
- RPM: або \u0394count за вікно (TIM6/TIM7 1 кГц → інтегрувати 10–50 мс), або input-capture (якщо є 1 канал “index”).

### 2.2 Handwheel (ручний енкодер)

Варіанти:
- **TIMy в Encoder mode** (кращий).
- Або EXTI на A/B (простий, але більше навантаження).

### 2.3 STEP генерація Z/X

Рекомендація (баланс складність/якість):
- STEP як **TIM OC toggle / PWM**, де таймер генерує імпульси, а код лише змінює ARR/CCR для прискорення/гальмування.

#### Оцінка частот у поточному AVR коді

З `7e2.ino`:
- **Timer2**: 16 МГц / 32 = **500 кГц**. Грубо \(f_{step} \approx 500000/(OCR2A+1)/2\). Для `OCR2A=35` це ~ **6.9 кГц**.
- **Timer5**: /1024 ⇒ **15.625 кГц** база (подача/конус).

Висновок: STEP частоти тут **порядку 1–10 кГц**, тому STM32 витягне і “IRQ+BSRR” (варіант A), але краще OC/PWM (варіант B).

Рівні реалізації:
- **A (швидкий старт)**: update IRQ → `GPIOx->BSRR` toggle.
- **B (рекомендовано)**: OC toggle/PWM + ISR змінює ARR/CCR.
- **C (пізніше)**: PWM + **DMA** профілю (мінімум джиттера).

### 2.4 Системний тик

- **TIM6/TIM7**: 1 кГц тик для debounce, RPM-вікна, UI-таймінгів (заміна `timer++`).

### 2.5 Типовий приклад розкладки (адаптувати під твою плату)

- **Spindle encoder**: TIM2 (Encoder mode) → A/B на CH1/CH2
- **Handwheel encoder**: TIM3 (Encoder mode) → A/B на CH1/CH2
- **Z_STEP**: TIM1 CH1 (PWM/OC toggle)
- **X_STEP**: TIM1 CH2 (PWM/OC toggle)
- **DIR/ENA/LED/BUZZER/кнопки**: GPIO
- **ADC pot**: один ADC канал

Як підібрати конкретні піни:
- В CubeMX вибираєш таймер → **Encoder Mode** / **PWM Generation** → CubeMX сам покаже доступні AF.
- Для сигналів з потенційними 5V рівнями: обирай **5V-tolerant** піни або роби рівнезміщення.
- Для ADC: тільки піни з ADC-функцією (вони **не** 5V tolerant).

### 2.6 Розпіновка для MCUDev Black STM32F407ZET6 (під `7e2` + LCD2004 I2C)

Розпіновка складена так, щоб **не займати** вузли плати (SWD, USB, SDIO, SPI Flash), як описано в [`mcauser/BLACK_F407ZE`](https://github.com/mcauser/BLACK_F407ZE).

#### Резерви плати (не використовувати під сигнали ELS)

- **SWD**: PA13 (SWDIO), PA14 (SWCLK)
- **USB**: PA11/PA12
- **microSD (SDIO)**: PC8–PC12 + PD2
- **SPI Flash**: PB3/PB4/PB5 + PB14 (CS)

#### STEP/DIR/ENA (осі)

- **Z_STEP**: PE9 = TIM1_CH1 (AF1)
- **X_STEP**: PE11 = TIM1_CH2 (AF1)
- **Z_DIR**: PE10 (GPIO)
- **X_DIR**: PE12 (GPIO)
- **Z_ENA**: PE8 (GPIO)
- **X_ENA**: PE7 (GPIO)

#### Spindle encoder (квадратура, Encoder mode)

- **Spindle_A**: PD12 = TIM4_CH1 (AF2)
- **Spindle_B**: PD13 = TIM4_CH2 (AF2)

#### Handwheel encoder (Encoder mode)

- **Hand_A**: PC6 = TIM3_CH1 (AF2)
- **Hand_B**: PC7 = TIM3_CH2 (AF2)

#### LCD 2004 I2C (PCF8574)

- **I2C2_SCL**: PB10 (AF4)
- **I2C2_SDA**: PB11 (AF4)

Важливо: якщо LCD живиться від **5V** і має підтяжки SDA/SCL до 5V — потрібен **I2C level shifter** (BSS138) або підтяжки мають бути **до 3.3V**.

#### UI входи (кнопки/перемикачі/джойстик)

- **Mode switch (8 біт)**: PC0..PC7 (GPIO in)
- **Joystick (4 біти)**: PG0..PG3 (GPIO in)
- **Rapid**: PG4 (GPIO in)
- **Submode (3 біти)**: PG5..PG7 (GPIO in)
- **Menu buttons (4 шт)**: PF0..PF3 (GPIO in)
- **Select**: PF4 (GPIO in)

#### Ліміти + LED

- **Limit inputs (4 шт)**: PB0, PB1, PB2, PB6 (GPIO in)
- **Limit LEDs (4 шт)**: PB7, PB8, PB9, PB12 (GPIO out)

#### Інші

- **TACHO_OUT**: PG13 (GPIO out)
- **BUZZER**: PD15 (GPIO out, бажано через транзистор/мосфет)
- **DIVIDER_OUT** (аналог Mega D53): PD14 (GPIO out)
- **ADC_POT**: PA4 (ADC1_IN4); потенціометр бажано живити від **3.3V**
- **UART лог (рекомендовано)**: USART1 TX/RX = PA9/PA10

## 3) Стратегія порту (CubeIDE, HAL/LL)

### 3.1 Розділити код на “алгоритми” і “драйвери”

У поточному коді алгоритм тісно змішаний з залізом (макроси `PORTx`, `OCRx`, enable/disable IRQ). Для STM32 варто зробити 2 шари:

- **HAL/LL-шар (низький рівень)**:
  - `io_*`: DIR/ENA/LED/buzzer, читання кнопок/перемикачів
  - `stepgen_*`: start/stop для осі, set_period/set_freq, профіль accel/decel
  - `encoder_*`: spindle/handwheel count + speed
  - `lcd_*`: HD44780 (GPIO або I2C)
- **Логіка режимів**: `Feed/Thread/Cone/Sphere/aFeed/Menu` — але замість прямої роботи з `OCR*` працюєш з абстракціями `stepgen_set_period(axis, ...)`, `encoder_get_count(...)`.

#### Рекомендований “каркас” файлів у CubeIDE-проєкті

- `Core/Src/app_main.c` / `Core/Inc/app_main.h` — аналог `loop()`, state-machine.
- `Core/Src/app_modes/*.c` — порт логіки режимів з `.ino`.
- `Core/Src/bsp_io.c/.h` — GPIO/входи/виходи.
- `Core/Src/bsp_stepgen.c/.h` — генерація STEP (TIM OC/PWM або IRQ+BSRR).
- `Core/Src/bsp_encoder.c/.h` — encoder mode + RPM.
- `Core/Src/ui_lcd.c/.h` — HD44780 або I2C LCD.
- `Core/Inc/app_config.h` — “параметри заліза” + pinmap для STM32.

### 3.2 Як відобразити AVR-таймери на STM32

У AVR різні режими включають різні ISR (`Ena_INT_*`). На STM32 концептуально:
- **Thread mode**: spindle encoder + stepgen у “sync-to-spindle” режимі.
- **Feed/Cone/aFeed/Rapid**: stepgen з власною частотою/профілем.
- **Handwheel**: stepgen тягнеться за handwheel position.

Практичний принцип: таймери/енкодери рахують **апаратно**, ISR залишити для легких оновлень параметрів та періодичних задач.

## 4) Чекліст 3.3V/5V (що перевірити перед переносом)

Ти вказав “mixed”, тому перед пайкою/переносом:

- **Енкодери**:
  - Який вихід (open-collector / push-pull)? Яка напруга живлення?
  - Якщо push-pull 5V → потрібен level shift або 5V-tolerant input.
- **LCD (HD44780)**:
  - Якщо паралельний 5V: 3.3V HIGH інколи “працює”, але не гарантується.
  - Безпечніше: рівнезміщувач/буфер або I2C backpack.
- **Кнопки/перемикачі**:
  - Якщо тягнуться до +5V — вхід має бути 5V tolerant або через дільник/оптрон.
- **STEP/DIR/ENA до драйверів**:
  - Більшість драйверів приймає 3.3V як HIGH, але не всі.
  - Якщо потрібні 5V рівні — буфер (HCT) або опто-вхід.
- **Зумер/реле/делилка**:
  - Якщо навантаження не “логічне” — керувати через транзистор/мосфет.

### 4.1 Типові “рецепти” узгодження рівнів (коротко)

- **5V → 3.3V (повільні сигнали: кнопки/перемикачі)**: резистивний дільник або інтерфейсний буфер.
- **5V → 3.3V (швидкі сигнали: енкодер)**: краще перетворювач рівнів/буфер (LVC/T-серії) або опто.
- **3.3V → 5V (LCD або STEP/DIR якщо треба “чисті” 5V)**: 74HCT-серія як “підсилювач” рівнів (HCT сприймає 3.3V як HIGH при 5V живленні).
- **Захист входів**: обов’язково звіряти з даташитом STM32F407 (не всі піни 5V tolerant, ADC-лінії — ні).

### 4.2 Які саме лінії в цьому проєкті потенційно “5V-ризикові”

Якщо зараз усе було на Arduino Mega (5V логіка), то при переході на STM32 (3.3V) в зоні ризику:

- **Входи (5V → 3.3V)**:
  - Spindle encoder **A/B** (швидкі).
  - Handwheel **A/B** (швидкі).
  - `Mode_Read` (8-біт перемикач), `Submode_Read` (3 біти), `Joy_Read` (4 біти), `Button_Rapid`, `Buttons_Read` + `Button_Sel_Read`, `Limit_Buttons_Read`, `Hand_Axis_Read`, `Hand_Scale_Read` (повільні).
  - ADC потенціометр (якщо “верх” потенціометра на 5V): **вхід ADC має бути ≤3.3V** (або живити потенціометр від 3.3V, або ставити дільник/буфер).

- **Виходи (3.3V → 5V)**:
  - STEP/DIR/ENA на драйвери осей (якщо драйвери вимагають 5V TTL).
  - LCD лінії (якщо паралельний HD44780 на 5V і потрібен гарантований HIGH).
  - Buzzer/“делилка” (якщо це не логічний вхід, а навантаження).

Мінімальна практика для безпеки:
- **Енкодери**: або зробити правильний level shift/буфер, або живити енкодер від 3.3V (якщо дозволяє), або використати open-collector з підтяжкою на 3.3V.
- **Потенціометр**: живити від 3.3V і обмежити діапазон до 0..3.3V.

## 5) Мінімальний план переносу (реалістичний порядок робіт)

1) Намалювати **фінальну таблицю пінів** під конкретний STM32F407ZET6-MODUL (що реально виведено на роз’єми).
2) Підняти CubeIDE: GPIO + UART log + TIM encoder (spindle) + TIM step (одна вісь).
3) Відтворити один режим (наприклад **Feed Z**) з генерацією STEP без пропусків.
4) Додати X вісь + rapid.
5) Додати handwheel (encoder mode) + “тягнути” осі за позицією.
6) Під’єднати LCD + меню.
7) Валідація різьби: тест різних RPM, перевірка кроку і повторюваності.

