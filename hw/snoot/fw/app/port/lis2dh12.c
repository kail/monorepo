#include "port/lis2dh12.h"

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
}
