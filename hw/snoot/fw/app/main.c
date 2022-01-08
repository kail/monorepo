#include "hal_config.h"
#include "port/lis2dh12.h"

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

int main() {
  hal_init();
  lis2dh12_init(&hi2c1, &lis2dh12_ctx_);
  lis2dh12_configure_interrupt(&lis2dh12_ctx_);

  // Spin
  while (1) {
    blink();
    print_accel_data();
    HAL_Delay(1000);
  }
}
