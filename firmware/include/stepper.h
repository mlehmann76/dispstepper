#ifndef __STEPPER_H_
#define __STEPPER_H_

#include <stdint.h>
#include <stdbool.h>

#define STEPS 8


class stepCtrl {
  public:
    enum dir_t { CW, CCW };

    stepCtrl(uint32_t stepsPerTurn, float timerFreq);
    void cw(float speed, uint32_t steps);
    void ccw(float speed, uint32_t steps);
    void func(uint32_t timerVal);
    bool isRunning() const;
    dir_t dir() const {return _dir;}

  private:
    float _timerFreq;
    uint32_t _stepsPerTurn;
    dir_t _dir;
    uint32_t _steps;
    uint32_t _lastTimerVal;
    uint32_t _timerValPerStep;
    uint8_t _stepIndex;
};


#endif // !__STEPPER_H_
