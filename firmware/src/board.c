#include <atmel_start.h>
#include <hal_gpio.h>
#include <nv_storage.h>

static volatile uint32_t tick;

void SysTick_Handler() { tick++; }

uint32_t getTick() { return tick; }

void SysTick_Init(uint32_t cpuFreq) {
  tick = 0;
  // Configure SysTick to trigger every millisecond using the CPU Clock
  SysTick->CTRL = 0; // Disable the SysTick Module
  SysTick->LOAD =
      (cpuFreq / 1000) - 1; // Set the Reload Register for 1mS interrupts
  NVIC_SetPriority(SysTick_IRQn,
                   3); // Set the interrupt priority to least urgency
  SysTick->VAL = 0;    // Clear the Current Value register
  SysTick->CTRL =
      0x00000007; // Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
  NVIC_EnableIRQ(SysTick_IRQn); // Enable the SysTick Interrupt (Global)
}

void board_init() {
  atmel_start_init();
  SysTick_Init(48e6);
}

void wrap_nv_storage_init() { nv_storage_init(); }

int32_t wrap_nv_storage_read(const uint16_t id, const uint16_t offset,
                             uint8_t *const data, const uint16_t size) {
  return nv_storage_read(id, offset, data, size);
}

uint16_t wrap_nv_storage_item_size(const uint16_t id) {
  return nv_storage_item_size(id);
}

int32_t wrap_nv_storage_write(const uint16_t id, const uint16_t offset,
                              const uint8_t *const data, const uint16_t size) {
  return nv_storage_write(id, offset, data, size);
}

void wrap_gpio_set_pin_level(const uint8_t pin, const bool level) {
  gpio_set_pin_level(pin, level);
}

bool wrap_gpio_get_pin_level(const uint8_t pin) {
  return gpio_get_pin_level(pin);
}

uint16_t readCounter() { return hri_tccount16_read_COUNT_reg(TC3); }
