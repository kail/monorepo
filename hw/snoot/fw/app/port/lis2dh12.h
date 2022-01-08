#pragma once

#include "stm32l1xx_hal.h"
#include "lis2dh12_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the port functionality required by the driver.
void lis2dh12_init(I2C_HandleTypeDef* hi2c, stmdev_ctx_t* dev_ctx);

#ifdef __cplusplus
}
#endif
