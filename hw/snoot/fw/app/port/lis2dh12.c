#include "port/lis2dh12.h"

#include <string.h>

// Turn this off if the accelerometer is broken so startup succeeds.
#define SELF_TEST 1
#define BOOT_TIME_MS 5

/** Please note that is MANDATORY: return 0 -> no Error.**/
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
  /* Write multiple command */
  reg |= 0x80;
  return HAL_I2C_Mem_Write(handle, LIS2DH12_I2C_ADD_L, reg,
                    I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
}

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
  /* Read multiple command */
  reg |= 0x80;
  return HAL_I2C_Mem_Read(handle, LIS2DH12_I2C_ADD_L, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
}

//
// Public Interface
//
void lis2dh12_init(I2C_HandleTypeDef* hi2c, stmdev_ctx_t* dev_ctx) {
  dev_ctx->write_reg = platform_write;
  dev_ctx->read_reg = platform_read;
  dev_ctx->handle = hi2c;

  // The device needs time to boot.
  HAL_Delay(BOOT_TIME_MS);

#if SELF_TEST
  uint8_t whoamI;

  // Validate that the correct device ID is being returned.
  lis2dh12_device_id_get(dev_ctx, &whoamI);
  if (whoamI != LIS2DH12_ID) {
    Error_Handler();
  }
#endif //SELF_TEST

  // Enable Block Data Update (BDU CTRL_REG4). This is required for enabling temperature.
  lis2dh12_block_data_update_set(dev_ctx, PROPERTY_ENABLE);
  
  // Set full scale to 2g. This should be plenty for detecting dog motion.
  lis2dh12_full_scale_set(dev_ctx, LIS2DH12_2g);

  // The temperature sensor on both boards is fried.
  // This one isn't very accurate, but it will have to do.
  lis2dh12_temperature_meas_set(dev_ctx, LIS2DH12_TEMP_ENABLE);

  // There are 2 main configs for power: the refresh rate (controlled by ODR)
  // and the operating mode (low-power, normal, and high-resolution).
  // Since accuracy isn't super important, normal (10 bit) and low-power (8 bit)
  // would both suffice. This configuration should draw ~4uA.
  lis2dh12_operating_mode_set(dev_ctx, LIS2DH12_LP_8bit);
  lis2dh12_data_rate_set(dev_ctx, LIS2DH12_ODR_25Hz);
}

bool lis2dh12_get_acceleration(stmdev_ctx_t* dev_ctx, lis2dh12_accel_t* accel) {
  lis2dh12_reg_t reg;
  int16_t data_raw_acceleration[3];

  // Read output only if new value available.
  lis2dh12_xl_data_ready_get(dev_ctx, &reg.byte);
  if (!reg.byte) {
    return false;
  }

  // Read accelerometer data.
  memset(data_raw_acceleration, 0x00, sizeof(data_raw_acceleration));
  lis2dh12_acceleration_raw_get(dev_ctx, data_raw_acceleration);
  
  // NOTE: These functions change with the scale and operating mode!
  accel->x_mg = lis2dh12_from_fs2_lp_to_mg(data_raw_acceleration[0]);
  accel->y_mg = lis2dh12_from_fs2_lp_to_mg(data_raw_acceleration[1]);
  accel->z_mg = lis2dh12_from_fs2_lp_to_mg(data_raw_acceleration[2]);
  return true;
}

bool lis2dh12_get_temperature(stmdev_ctx_t* dev_ctx, float* temp_c) {
  lis2dh12_reg_t reg;
  int16_t raw_temperature;
  lis2dh12_temp_data_ready_get(dev_ctx, &reg.byte);

  if (!reg.byte) {
    return false;
  }
  
  // Read temperature data.
  memset(&raw_temperature, 0x00, sizeof(raw_temperature));
  lis2dh12_temperature_raw_get(dev_ctx, &raw_temperature);

  // NOTE: This function changes with the operating mode!
  *temp_c = lis2dh12_from_lsb_lp_to_celsius(raw_temperature);
  return true;
}
