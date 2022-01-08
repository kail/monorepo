#pragma once

#include "hal_config.h"
#include "lis2dh12_reg.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float x_mg;
  float y_mg;
  float z_mg;
} lis2dh12_accel_t;

/* Initialize the port functionality required by the driver. */
void lis2dh12_init(I2C_HandleTypeDef* hi2c, stmdev_ctx_t* dev_ctx);

/* Get the current acceleration data. Returns false if not available; true otherwise. */
bool lis2dh12_get_acceleration(stmdev_ctx_t* dev_ctx, lis2dh12_accel_t* accel);

/* Get the current temperature data. Returns false if not available; true otherwise. */
bool lis2dh12_get_temperature(stmdev_ctx_t* dev_ctx, float* temp_c);

#ifdef __cplusplus
}
#endif
