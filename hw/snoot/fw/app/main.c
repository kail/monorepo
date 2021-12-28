#include "hal_config.h"
#include <stdbool.h>

void blink() {
  static bool led_on_ = false;
  if (led_on_) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	}
	led_on_ = !led_on_;
}

int main() {
  hal_init();

  // Spin
  while (1) {
    blink();
    HAL_Delay(1000);
  }
}
