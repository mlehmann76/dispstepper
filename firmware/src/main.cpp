#include <stdint.h>
#include "board.h"
#include "stepper.h"
#include <atmel_start.h>

typedef enum { SIDLE, SRUN, SRETRACT } state_t;
static state_t sstate;
#define BUTTON_STEPS 256

int main(void) {
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();
  SysTick_Init();
  timer_start(&TIMER_0);

  stepCtrl step = {4096, 1e6};
  keycheck<uint8_t, SW_MODE, SW_DOWN, SW_SEL, SW_UP> buttons;

  /* Replace with your application code */
  uint32_t lastTick = 0;
  while (1) {
    if (lastTick != getTick()) {
      lastTick = getTick();
      buttons.run();
    }
    step.func(hri_tccount16_read_COUNT_COUNT_bf(TC3));
    switch (sstate) {
    case SIDLE:
      if (buttons.pressed<SW_UP>()) {
        step.cw(0.2, BUTTON_STEPS);
        sstate = SRUN;
      } else if (buttons.pressed<SW_DOWN>()) {
        step.ccw(0.2, BUTTON_STEPS);
        sstate = SRUN;
      }
      break;
    case SRUN:
      if (!step.isRunning()) {
        if (step.dir() == stepCtrl::CW) {
          step.ccw(0.2, BUTTON_STEPS / 16);
        } else {
          step.cw(0.2, BUTTON_STEPS / 16);
        }
        sstate = SRETRACT;
      }
      break;
    case SRETRACT:
      if (!step.isRunning()) {
        sstate = SIDLE;
      }
      break;
    }
  }
}
