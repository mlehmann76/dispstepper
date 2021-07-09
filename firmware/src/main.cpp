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
using ledViewType = ledview<uint8_t, LED5, LED4, LED3, LED2>;
using modeType = mode<uint8_t, buttonCheckType, ledViewType>;

using namespace std::placeholders;

int main(void) {
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();
  SysTick_Init(48e6);

  stepCtrl step = {4096, 3e6};
  modeType mode;
  control ctrl = {&step};
  mode.registerModeCb(std::bind(&control::onModeChange, &ctrl, _1, _2));
  mode.registerButtonCb(std::bind(&control::onButtonChange<uint8_t>, &ctrl, _1, _2));
  gpio_set_pin_level(nSLEEP, 1);

  /* Replace with your application code */
  while (1) {
    mode.run(getTick());
    ctrl.run();
    step.func(hri_tccount16_read_COUNT_reg(TC3));
  }
}