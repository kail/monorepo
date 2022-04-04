#include "hal_config.h"
#include "port/lis2dh12.h"
#include "port/ltr390.h"
#include "time.h"
#include "tusb.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//
// External declarations.
// TODO: these should come from a board interface.
//
extern I2C_HandleTypeDef hi2c1;

//
// Private variables
//
static stmdev_ctx_t lis2dh12_ctx_;

void blink() {
  static bool led_on_ = false;
  if (led_on_) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  }
  led_on_ = !led_on_;
  printf("LED: %d\r\n", led_on_);
}

void print_accel_data() {
  uint8_t whoamI;
  lis2dh12_accel_t accel_data;
  static float temperature_degC;
  lis2dh12_device_id_get(&lis2dh12_ctx_, &whoamI);
  printf("Accel whoamI: 0x%x; Should be: 0x%x\r\n", whoamI, LIS2DH12_ID);

  if (lis2dh12_get_acceleration(&lis2dh12_ctx_, &accel_data)) {
    printf("Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n", accel_data.x_mg, accel_data.y_mg, accel_data.z_mg);
  }

  if (lis2dh12_get_temperature(&lis2dh12_ctx_, &temperature_degC)) {
    printf("Temperature [degC]:%6.2f\r\n", temperature_degC);
  }

  // Read IN1_SRC to clear the interrupt
  lis2dh12_int1_src_t src;
  lis2dh12_int1_gen_source_get(&lis2dh12_ctx_, &src);
}

void print_als_data() {
  static ltr390_reg_als_data_s als_data;
  static ltr390_reg_part_id_u part;
  if (!ltr390_get_part_id(&part)) {
    printf("LTR-390 part ID failed\n");
  } else {
    printf("LTR-390 part ID: %d - %d\n", part.part_id, part.revision_id);
    if (!ltr390_get_als_data(&als_data)) {
      printf("LTR-390 get ALS data failed\n");
      return;
    }
    printf("LTR-390 ALS data: %X %X %X\n", (als_data.high & 0xF), als_data.middle, als_data.low);
  }
}

int main() {
  hal_init();
  tusb_init();
  lis2dh12_init(&hi2c1, &lis2dh12_ctx_);
  if (!lis2dh12_configure_interrupt(&lis2dh12_ctx_)) {
    printf("Failed to configure lis2dh12 interrupts\n");
  }

  ltr390_init(&hi2c1);
  if (ltr390_configure()) {
    printf("LTR-390 config failed\n");
  }

  // Spin indefinitely
  uint32_t print_time = 0;
  while (1) {
    if (time_since_previous_ms(print_time) > 1000) {
      print_time = time_current_ms();
      blink();
      print_accel_data();
      print_als_data();
    }

    tud_task(); // tinyusb device task
  }
}
