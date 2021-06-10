#include "board.h"
#include "control.h"
#include "leds.h"
#include "mode.h"
#include "stepper.h"
#include "user_board.h"
#include <atmel_start.h>
#include <functional>
#include <stdint.h>

using buttonCheckType = buttonCheck<uint8_t, SW_MODE, SW_DOWN, SW_SEL, SW_UP>;
using ledViewType = ledview<uint8_t, LED2, LED3, LED4, LED5>;
using modeType = mode<uint8_t, buttonCheckType, ledViewType>;

using namespace std::placeholders;

int main(void) {
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();
  SysTick_Init();
  timer_start(&TIMER_0);

  stepCtrl step = {4096, 1e6};
  buttonCheckType buttons;
  ledViewType leds;
  modeType mode;
  control ctrl = {&step};
  mode.registerModeCb(std::bind(&control::onModeChange, &ctrl, _1, _2));
  mode.registerButtonCb(std::bind(&control::onButtonChange<uint8_t>, &ctrl, _1, _2));

  /* Replace with your application code */
  while (1) {
    buttons.run(getTick());
    mode.run(getTick());
    ctrl.run();
    step.func(hri_tccount16_read_COUNT_COUNT_bf(TC3));
  }
}
