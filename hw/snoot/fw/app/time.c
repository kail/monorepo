#include "time.h"

#include "conf.h"
#include "stm32l1xx_hal.h"

#define MILLIS_IN_SECOND (1000)
#define TICK_TO_MS(t) (t * MILLIS_IN_SECOND / CONF_TICKS_PER_SECOND)
#define MAX_TIME ((uint32_t)0xFFFFFFFFUL)

uint32_t time_current_ms() {
  return TICK_TO_MS(HAL_GetTick());
}

uint32_t time_since_previous_ms(uint32_t previous) {
  uint32_t current = TICK_TO_MS(HAL_GetTick());

  // Wraparound case
  if (current < previous) {
    return MAX_TIME - previous + current;
  }
  return current - previous;
}
