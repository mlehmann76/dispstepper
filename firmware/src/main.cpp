#include "board.h"
#include "cmdparse.h"
#include "config.h"
#include "control.h"
#include "leds.h"
#include "mode.h"
#include "simpleparser.h"
#include "stepper.h"
#include "usb_cdc.h"
#include "user_board.h"
#include <atmel_start.h>
#include <functional>
#include <stdint.h>

using buttonCheckType = buttonCheck<uint8_t, SW_MODE, SW_DOWN, SW_SEL, SW_UP>;
using ledViewType = ledview<uint8_t, LED5, LED4, LED3, LED2>;
using modeType = mode<uint8_t, buttonCheckType, ledViewType>;

using namespace std::placeholders;
using link_type = simpleparser::KeywordPatternLink<char, 4>;

std::array<link_type, 2> scpi_cmd = {
    link_type{"*IDN", []() { return "dispstepper_v0"; },
              [](std::string_view s) {}},
    link_type{"TEST", []() { return "test success"; },
              [](std::string_view s) {}}};

int main(void) {
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();
  cdcd_acm_example();
  SysTick_Init(48e6);

  Config config;
  CmdParse parser = {&config};
  stepCtrl step = {4096, 3e6};
  modeType mode(&config);
  control ctrl = {&config, &step};
  mode.registerModeCb(std::bind(&control::onModeChange, &ctrl, _1, _2));
  mode.registerButtonCb(
      std::bind(&control::onButtonChange<uint8_t>, &ctrl, _1, _2));
  gpio_set_pin_level(nSLEEP, 1);

  while (1) {
    char rec;
    size_t recLen;
    //
    config.run(getTick());
    mode.run(getTick());
    ctrl.run();
    step.func(hri_tccount16_read_COUNT_reg(TC3));
    usb_service(&rec, &recLen);
    if (recLen) {
      parser.push(rec);
    }
    parser.service(scpi_cmd);
  }
}
