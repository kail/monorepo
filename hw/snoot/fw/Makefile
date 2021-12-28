#
# Project Name
#
PROJECT = snoot
VERSION = v1

#
# Build Variables
#
DEBUG = 1
OPT = -Og

#
# Project Paths
#
BUILD_DIR=./bin
OBJ_DIR = $(BUILD_DIR)/objs
APP_DIR=./app
BOARD_DIR=./board/$(VERSION)
VENDOR_FW=../../../vendor/fw

#
# Toolchain
#

# Must be installed and in PATH
PREFIX = arm-none-eabi-
AS = $(PREFIX)gcc -x assembler-with-cpp
GCC = $(PREFIX)gcc
GDB = $(PREFIX)gdb-py
OBJCOPY = $(PREFIX)objcopy
SZ = $(PREFIX)size

# OpenOCD
OCD=openocd

#
# Compiler and Linker Flags
# 

# All warnings
CFLAGS = -Wall
# Treat warnings as errors
CFLAGS += -Werror
# Generate ELF section for each var in source file
CFLAGS += -fdata-sections
# Generate a section for each function. Use with 
# --gc-sections so that unused sections are removed!
CFLAGS += -ffunction-sections
# Colors are neat
CFLAGS += -fdiagnostics-color=always
# Use the C99 language standard
CFLAGS += -std=c99
# Assert that compilation targets a freestanding environment
CFLAGS += -ffreestanding

ifeq ($(DEBUG), 1)
CFLAGS += -g
endif

# Use newlib-nano C libraries
# Use -specs=nosys.specs if using system libs, no system stubs
LDFLAGS = -specs=nano.specs
# Remove any unused sections
LDFLAGS += -Wl,--gc-sections
# Pretty-print memory usage
LDFLAGS += -Wl,--print-memory-usage

# Link libc
LDLIB = -lc
# Link libm
LDLIB += -lm
# Link default system calls
LDLIB += -lnosys

#
# STM32 L1 Cube
#
STM32L1_DIR=$(VENDOR_FW)/STM32CubeL1
STM32L1_SRCDIR=$(STM32L1_HAL)/Src
STM32L1_HAL=$(STM32L1_DIR)/Drivers/STM32L1xx_HAL_Driver
STM32L1_CMSIS=$(STM32L1_DIR)/Drivers/CMSIS
CMSIS_INC = $(STM32L1_CMSIS)/Include
STM32L1_INC = $(STM32L1_HAL)/Inc/
STM32L1_INC += $(STM32L1_HAL)/Inc/Legacy/
STM32L1_SRC = $(wildcard $(STM32L1_SRCDIR)/*.c)
STM32L1_OBJ = $(patsubst $(STM32L1_SRCDIR)/%.c,$(OBJ_DIR)/%.o,$(STM32L1_SRC))

#
# Board-specific Settings
#
ifeq ($(VERSION), v1)
	CPU = -mcpu=cortex-m3
	FPU =
	FLOAT-ABI = -mfloat-abi=soft
	MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
	ST_C_DEFS = USE_HAL_DRIVER STM32L151xBA
	BOARD_INC = $(BOARD_DIR)
	BOARD_SRC = $(wildcard $(BOARD_DIR)/*.c)
	CMSIS_INC += $(STM32L1_CMSIS)/Device/ST/STM32L1xx/Include
	LDFLAGS += -T$(BOARD_DIR)/STM32L151C8TxA_FLASH.ld
	STARTUP = $(BOARD_DIR)/startup_stm32l151xba.s
endif

#
# Source Search Paths
#
VPATH = $(APP_DIR)
VPATH += $(BOARD_DIR)
VPATH += $(STM32L1_SRCDIR)

#
# Application
#

# TODO: Make separate makefiles per application
APP_SRCS = $(wildcard $(APP_DIR)/*.c)
APP_INC = $(APP_DIR)
APP_DEFS = 

#
# Final Command Flags
#
SRCS = $(APP_SRCS) $(BOARD_SRC) $(STM32L1_SRC)
INC_DIRS = $(STM32L1_INC) $(CMSIS_INC) $(APP_INC) $(BOARD_INC)
DEFS = $(APP_DEFS) $(ST_C_DEFS)

C_INC = $(foreach i,$(INC_DIRS),-I$(i))
C_DEFS += $(foreach d,$(DEFS),-D$(d))

OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
OBJS += $(addprefix $(OBJ_DIR)/,$(notdir $(STARTUP:.s=.o)))
CFLAGS += $(MCU) $(C_DEFS) $(C_INC) $(OPT)

#
# Build Targets
#
all: $(BUILD_DIR)/$(PROJECT).elf

dirs: $(BUILD_DIR) $(OBJ_DIR)
$(BUILD_DIR) $(OBJ_DIR):
	@echo "Making output dir $@"
	@mkdir -p $@

$(OBJ_DIR)/%.o : %.c | dirs
	@echo "Compiling $(notdir $<)"
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o : %.s | dirs
	@echo "Assembling $(notdir $<)"
	$(AS) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@echo "Linking $@"
	$(GCC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*