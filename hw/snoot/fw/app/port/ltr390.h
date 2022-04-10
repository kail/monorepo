#pragma once

#include "hal_config.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  uint8_t data_u8;
  struct {
    uint8_t revision_id : 4;
    uint8_t part_id : 4;
  };
} ltr390_reg_part_id_u;

typedef union {
  struct {
    uint8_t low;
    uint8_t middle;
    uint8_t high;
  };
  uint32_t data;
} ltr390_reg_als_data_s;

/** Initialize the LTR-390 driver. */
void ltr390_init(I2C_HandleTypeDef* hi2c);

/** Configure the part to its default settings. */
bool ltr390_configure();

/** Returns the part ID and revision info. */
bool ltr390_get_part_id(ltr390_reg_part_id_u* output);

/** Return the ALS data. */
bool ltr390_get_als_data(ltr390_reg_als_data_s* output);

/** Calculate the lux based on datasheet equation (pg.23). */
static inline float calculate_lux(uint32_t als_data, uint8_t gain, float int_time, float w_fac) {
  return (0.6 * (float)als_data) / (int_time * (float)gain) * w_fac;
}

#ifdef __cplusplus
}
#endif
