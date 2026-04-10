# GNU Make + arm-none-eabi-gcc (STM32Cube CLT / GCC ARM)
TARGET = els_f407_black

PREFIX ?= arm-none-eabi-
CC := $(PREFIX)gcc
AS := $(PREFIX)as
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

DEFS = -DUSE_HAL_DRIVER -DSTM32F407xx

INCLUDES = \
	-IInc \
	-ICore/Inc \
	-IDrivers/CMSIS/Include \
	-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
	-IDrivers/STM32F4xx_HAL_Driver/Inc \
	-IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy

CFLAGS = $(CPU) $(DEFS) $(INCLUDES) -Og -g3 -Wall -ffunction-sections -fdata-sections
CFLAGS += -std=c11 -Wno-unused-parameter

ASFLAGS = $(CPU) -g3

LDFLAGS = $(CPU) -specs=nano.specs -TSTM32F407ZETx_FLASH.ld
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDFLAGS += -lc -lm -lnosys

BUILD_DIR = build

APP_C = \
	Core/Src/app_main.c \
	Core/Src/bsp_ui_inputs.c \
	Core/Src/ui_menu.c \
	Core/Src/bsp_encoder.c \
	Core/Src/bsp_i2c.c \
	Core/Src/bsp_stepgen.c \
	Core/Src/bsp_uart.c \
	Core/Src/ui_lcd_pcf8574.c \
	Core/Src/sysmem.c

SRC_C = \
	Src/main.c \
	Src/gpio.c \
	Src/adc.c \
	Src/i2c.c \
	Src/tim.c \
	Src/usart.c \
	Src/stm32f4xx_it.c \
	Src/stm32f4xx_hal_msp.c \
	Src/system_stm32f4xx.c

HAL_C = \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
	Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c

ASM = Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s

SOURCES = $(APP_C) $(SRC_C) $(HAL_C) $(ASM)
CSRCS = $(filter %.c,$(SOURCES))
ASRCS = $(filter %.s,$(SOURCES))
OBJECTS = $(addprefix $(BUILD_DIR)/,$(patsubst %.c,%.o,$(notdir $(CSRCS))))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(patsubst %.s,%.o,$(notdir $(ASRCS))))
VPATH = $(sort $(dir $(CSRCS) $(ASRCS)))

ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = $(BUILD_DIR)/$(TARGET).bin
HEX = $(BUILD_DIR)/$(TARGET).hex

.PHONY: all clean

all: $(ELF) $(BIN)
	$(SIZE) $(ELF)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS) STM32F407ZETx_FLASH.ld
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
