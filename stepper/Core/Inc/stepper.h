#ifndef __STEPPER_H_
#define __STEPPER_H_

#include <stdint.h>
#include <stdbool.h>

#define STEPS 8

typedef enum {CW,CCW} dir_t;

typedef struct {
  GPIO_TypeDef *gpio;
  uint16_t pin;
  GPIO_PinState state;
} gpioState_t;

typedef struct {
  float _timerFreq;
  uint32_t _stepsPerTurn;
  dir_t _dir;
  uint32_t _steps;
  uint32_t _lastTimerVal;
  uint32_t _timerValPerStep;
  uint8_t _stepIndex;
} stepCtx_t;

void step_init(stepCtx_t *pCtx, uint32_t stepsPerTurn, float timerFreq);
void step_cw(stepCtx_t *pCtx, float speed, uint32_t steps);
void step_ccw(stepCtx_t *pCtx, float speed, uint32_t steps);
void step_func(stepCtx_t *pCtx, uint32_t timerVal);
bool step_isRunning(stepCtx_t *pCtx);

#endif // !__STEPPER_H_
