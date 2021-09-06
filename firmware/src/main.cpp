#include "board.h"
#include "button.h"
#include "cmdparse.h"
#include "config.h"
#include "control.h"
#include "leds.h"
#include "mode.h"
#include "simpleparser.h"
#include "stepper.h"
#include "usb_cdc_wrap.h"
#include <stdint.h>

using buttonCheckType = buttonCheck<uint8_t, SW_MODE, SW_DOWN, SW_SEL, SW_UP>;
using ledViewType = ledview<uint8_t, LED5, LED4, LED3, LED2>;
using modeType = mode<uint8_t, buttonCheckType, ledViewType>;

using namespace std::placeholders;

int main(void) {
  /* Initializes MCU, drivers and middleware */
  board_init();

  Config config;
  usb_cdc_wrapper cdc;
  CmdParse parser = {config, cdc};
  stepCtrl step = {4096, 3e6};
  modeType mode(&config);
  control ctrl = {&config, &step};
  mode.registerModeCb(std::bind(&control::onModeChange, &ctrl, _1, _2));
  mode.registerButtonCb(
      std::bind(&control::onButtonChange<uint8_t>, &ctrl, _1, _2));
  cdc.set(std::bind(&CmdParse::push, &parser, _1));
  wrap_gpio_set_pin_level(nSLEEP, 1);

  while (1) {
    //
    config.run(getTick());
    mode.run(getTick());
    ctrl.run();
    step.func(readCounter());
    //
    cdc.read();
    //
    parser.service();
  }
}
