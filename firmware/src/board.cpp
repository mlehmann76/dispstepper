#include <atmel_start.h>

static volatile uint32_t tick;

void SysTick_Handler() {
    tick++;
}

uint32_t getTick() {
    return tick;
}

void SysTick_Init() {
tick = 0;
  // Configure SysTick to trigger every millisecond using the CPU Clock
  SysTick->CTRL = 0;     // Disable the SysTick Module
  SysTick->LOAD = 4799UL; // Set the Reload Register for 1mS interrupts
  NVIC_SetPriority(SysTick_IRQn,
                   3); // Set the interrupt priority to least urgency
  SysTick->VAL = 0;    // Clear the Current Value register
  SysTick->CTRL =
      0x00000007; // Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
  NVIC_EnableIRQ(SysTick_IRQn); // Enable the SysTick Interrupt (Global)
}