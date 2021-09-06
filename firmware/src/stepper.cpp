#include "stepper.h"
#include "board.h"
#include <stdint.h>

typedef struct {
  uint8_t gpio;
  bool state;
} gpioState_t;

static const gpioState_t state[STEPS][4] = {
    {{AIN1, 0}, {AIN2, 1}, {BIN2, 0}, {BIN1, 1}},
    {{AIN1, 0}, {AIN2, 1}, {BIN2, 0}, {BIN1, 1}},
    //
    {{AIN1, 1}, {AIN2, 1}, {BIN2, 0}, {BIN1, 0}},
    {{AIN1, 1}, {AIN2, 1}, {BIN2, 0}, {BIN1, 0}},
    //
    {{AIN1, 1}, {AIN2, 0}, {BIN2, 1}, {BIN1, 0}},
    {{AIN1, 1}, {AIN2, 0}, {BIN2, 1}, {BIN1, 0}},
    //
    {{AIN1, 0}, {AIN2, 0}, {BIN2, 1}, {BIN1, 1}},
    {{AIN1, 0}, {AIN2, 0}, {BIN2, 1}, {BIN1, 1}},
};

static void setStep(int num) {
  if (0 <= num && num < STEPS) {
    for (uint_fast8_t i = 0; i < 4; i++) {
      wrap_gpio_set_pin_level(state[num][i].gpio, state[num][i].state);
    }
  } else {
    for (uint_fast8_t i = 0; i < 4; i++) {
      wrap_gpio_set_pin_level(state[0][i].gpio, 0);
    }
  }
}

stepCtrl::stepCtrl(uint32_t stepsPerTurn, float timerFreq) {
  _timerFreq = timerFreq;
  _stepsPerTurn = stepsPerTurn;
  _steps = 0;
  _lastTimerVal = 0;
  _timerValPerStep = 0;
  _stepIndex = 0;
}

void stepCtrl::cw(float speedHz, uint32_t steps) {
  _steps = steps;
  _dir = CW;
  _lastTimerVal = 0;
  _timerValPerStep = _timerFreq / (speedHz * _stepsPerTurn);
}

void stepCtrl::ccw(float speedHz, uint32_t steps) {
  _steps = steps;
  _dir = CCW;
  _lastTimerVal = 0;
  _timerValPerStep = _timerFreq / (speedHz * _stepsPerTurn);
}

void stepCtrl::func(uint16_t timerVal) {
  if (_steps) {
    if (_lastTimerVal == 0) {
      _lastTimerVal = timerVal; // update TimerVal after start
    } else {
      if ((uint16_t)(timerVal - _lastTimerVal) > _timerValPerStep) {
        _lastTimerVal = timerVal;
        setStep(_stepIndex);
        if (_dir == CCW) {
          _stepIndex = _stepIndex == 7 ? 0 : _stepIndex + 1;
        } else {
          _stepIndex = _stepIndex == 0 ? 7 : _stepIndex - 1;
        }
        _steps -= 1;
      }
    }
  } else {
    setStep(-1);
  }
}

