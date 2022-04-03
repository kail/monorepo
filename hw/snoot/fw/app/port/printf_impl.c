#include "hal_config.h"
#include "tusb.h"
#include <stdbool.h>

#define PRINT_BUF_SIZE 512

// TODO: This should be passed in via constructor or in HW init
extern UART_HandleTypeDef huart1;

// Double-buffer used instead of a circular buffer for easier implementation
static uint8_t cbuf_[2][PRINT_BUF_SIZE];

// The buffer currently being written to
static volatile uint8_t write_buf_idx_ = 2;

// The write index within the buffer
static volatile uint8_t write_idx_ = 0;

/** This is called when a UART transmit completes via interrupt. */
void char_tx_complete() {
  if (write_idx_ > 0) {
    HAL_UART_Transmit_IT(&huart1, cbuf_[write_buf_idx_], write_idx_);
    write_buf_idx_ = (write_buf_idx_ == 0) ? 1 : 0;
  } else {
    write_buf_idx_ = 2;
  }
  write_idx_ = 0;
}

/** Output a character to the debug UART port */
void putchar_(char character) {
  // Note: this doesn't actually work due to the size of the buffer.
  // It was put here just for testing purposes. Realistically this will
  // need an RTOS to periodically call flush, and block the print task.
  // tud_cdc_n_write_char(0, character);
  // tud_cdc_n_write_flush(0);

  while (write_idx_ >= PRINT_BUF_SIZE) {
    // Can't write anything, spin!
    HAL_UART_StateTypeDef state = HAL_UART_GetState(&huart1);
    switch (state) {
    case HAL_UART_STATE_BUSY:
    case HAL_UART_STATE_BUSY_TX:
    case HAL_UART_STATE_BUSY_TX_RX:
      // It's just sending data
      continue;
    case HAL_UART_STATE_READY:
      if (write_idx_ > 0) {
        // It finished sending data, but we didn't get notified
        char_tx_complete();
      }
      break;
    default:
      // Something went wrong. Reset?
      // TODO: reset the peripheral
      break;
    }
  }
  
  bool tx_in_progress = false;

  // Disable all interrupts so that the UART TX callback doesn't modify the indices.
  __disable_irq();

  if (write_buf_idx_ <= 1) {
    // Transmit in progress; just copy data into the buffer.
    cbuf_[write_buf_idx_][write_idx_++] = character;
    tx_in_progress = true;
  }

  // Enable interrupts
  __enable_irq();
  
  if (!tx_in_progress) {
    // Need to pull this off the stack into a buffer
    cbuf_[0][0] = character;
    write_buf_idx_ = 1;
    write_idx_ = 0;
    if (HAL_UART_Transmit_IT(&huart1, cbuf_[0], 1) != HAL_OK) {
      // TODO: wait until next char? Reset?
    }
  }
}
