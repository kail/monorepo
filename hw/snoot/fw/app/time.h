#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t time_current_ms();

uint32_t time_since_previous_ms(uint32_t previous);

#ifdef __cplusplus
}
#endif
