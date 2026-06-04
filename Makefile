TARGET = greenhouseController

# Toolchain
CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc -x assembler-with-cpp
CP      = arm-none-eabi-objcopy
SZ      = arm-none-eabi-size

# Paths — update these to match where you saved the CMSIS headers
CMSIS_DEVICE = C:/STM32/cmsis-device-f4/Include
CMSIS_CORE   = C:/STM32/CMSIS_5/CMSIS/Core/Include

# Sources
C_SOURCES   = $(wildcard src/*.c)
ASM_SOURCES = startup/startup_stm32f446xx.s

# Compiler flags
CPU     = -mcpu=cortex-m4
FPU     = -mfpu=fpv4-sp-d16
FLOAT   = -mfloat-abi=hard
MCU     = $(CPU) -mthumb $(FPU) $(FLOAT)

CFLAGS  = $(MCU) -DSTM32F446xx -DUSE_FULL_LL_DRIVER
CFLAGS += -I$(CMSIS_DEVICE) -I$(CMSIS_CORE)
CFLAGS += -I src
CFLAGS += -Wall -Wextra -O0 -g3
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS = $(MCU) -T linker/STM32F446RETx_FLASH.ld
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=build/$(TARGET).map,--cref
LDFLAGS += --specs=nano.specs

# Build output
BUILD_DIR = build
OBJECTS   = $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o)) \
            $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))

# Rules
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SZ) $<

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(CP) -O binary $< $@

$(BUILD_DIR)/src/syscalls.o: src/syscalls.c
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(CC) $(CFLAGS) -Wno-unused-parameter -c $< -o $@

$(BUILD_DIR)/src/%.o: src/%.c
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/startup/%.o: startup/%.s
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(AS) $(MCU) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean