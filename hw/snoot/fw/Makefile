#
# Project Name
#
PROJ = snoot
VERSION = v1

#
# Build Variables
#
DEBUG = 1
OPT = -Og

#
# Project Paths
#
BIN_DIR=./bin
OBJ_DIR = $(BIN_DIR)/objs
APP_DIR=./app
BOARD_DIR=./board/$(VERSION)
VENDOR_FW=../../../vendor/fw

#
# Toolchain
#

# Must be installed and in PATH
PREFIX = arm-none-eabi-
AS = $(PREFIX)as
GCC = $(PREFIX)gcc
GDB = $(PREFIX)gdb-py
LD = $(PREFIX)gcc
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

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
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
	ST_C_DEFS = -DUSE_HAL_DRIVER -DSTM32L151xBA
	BOARD_INC = $(BOARD_DIR)
	CMSIS_INC += $(STM32L1_CMSIS)/Device/ST/STM32L1xx/Include
endif

# Source search paths
VPATH = ./app
VPATH += $(STM32L1_SRCDIR)

#
# Application
#

# TODO: Make separate makefiles per application
APP_SRCS = $(wildcard $(APP_DIR)/*.c)
APP_INC = $(APP_DIR)

SRCS = $(APP_SRCS) $(STM32L1_SRC)
INC_DIRS = $(STM32L1_INC) $(CMSIS_INC) $(APP_INC) $(BOARD_INC)
INC = $(INC_DIRS:%=-I%)
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
CFLAGS += $(MCU) $(ST_C_DEFS) $(INC) $(OPT)

all: $(OBJS)

dirs: $(BIN_DIR) $(OBJ_DIR)
$(BIN_DIR) $(OBJ_DIR):
	@echo "[MKDIR]   $@"
	@mkdir -p $@

$(OBJ_DIR)/%.o : %.c | dirs
	@echo "[CC]      $(notdir $<)"
	$(GCC) $(CFLAGS) -c -o $@ $< -MMD

# STM32 HAL
# $(STM32L1_OBJ): $(OBJ_DIR)/%.o : $(STM32L1_SRCDIR)/%.c $(STM32L1_INC)
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(BIN)/*