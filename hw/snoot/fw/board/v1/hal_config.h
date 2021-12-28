#pragma once

#include "stm32l1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the peripherals
void hal_init(void);

// Global error handler
void Error_Handler(void);

#ifdef __cplusplus
}
#endif