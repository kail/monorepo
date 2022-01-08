#include "hal_config.h"
#include "port/lis2dh12.h"

#include <stdbool.h>
#include <stdio.h>

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
  lis2dh12_device_id_get(&lis2dh12_ctx_, &whoamI);
  printf("Accel whoamI: %d\r\n", whoamI);
}

int main() {
  hal_init();
  lis2dh12_init(&hi2c1, &lis2dh12_ctx_);

  // Spin
  while (1) {
    blink();
    print_accel_data();
    HAL_Delay(1000);
  }
}
