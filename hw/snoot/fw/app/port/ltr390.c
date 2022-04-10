#include "ltr390.h"

#define LTR390_I2C_ADDR (0xA6)
#define LTR390_I2C_DEFAULT_TIMEOUT_MS (500)
#define LTR390_MAIN_SETTING (0x02)

//
// Registers
//
#define LTR390_REG_MAIN_CTRL (0x00)
#define LTR390_REG_PART_ID (0x06)
#define LTR390_REG_ALS_DATA_0 (0x0D)
#define LTR390_REG_ALS_DATA_1 (0x0E)
#define LTR390_REG_ALS_DATA_2 (0x0F)

//
// Private implementation
//
static I2C_HandleTypeDef* i2c_handle_;

static bool write_reg(uint8_t reg, const uint8_t *bufp, uint16_t len) {
  /* Write multiple command */
  return HAL_I2C_Mem_Write(i2c_handle_, LTR390_I2C_ADDR, reg,
                    I2C_MEMADD_SIZE_8BIT, (uint8_t*)bufp, len, LTR390_I2C_DEFAULT_TIMEOUT_MS) == HAL_OK;
}

static bool read_reg(uint8_t reg, uint8_t *bufp, uint16_t len) {
  /* Read multiple command */
  return HAL_I2C_Mem_Read(i2c_handle_, LTR390_I2C_ADDR, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, LTR390_I2C_DEFAULT_TIMEOUT_MS) == HAL_OK;
}

//
// Public interface
//
void ltr390_init(I2C_HandleTypeDef* hi2c) {
  i2c_handle_ = hi2c;
}

bool ltr390_configure() {
  uint8_t setting = LTR390_MAIN_SETTING;
  return write_reg(LTR390_REG_MAIN_CTRL, &setting, 1);
}

bool ltr390_get_part_id(ltr390_reg_part_id_u* output) {
  return read_reg(LTR390_REG_MAIN_CTRL, (uint8_t*)output, 1);
}

bool ltr390_get_als_data(ltr390_reg_als_data_s* output) {
  return read_reg(LTR390_REG_ALS_DATA_0, (uint8_t*)output, 3);
}
