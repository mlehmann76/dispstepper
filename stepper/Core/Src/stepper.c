#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "config.h"
#include "stepper.h"

static const gpioState_t state[STEPS][4] = {
    {{W0_PORT, W0_PIN, 0},{W1_PORT, W1_PIN, 0},{W2_PORT, W2_PIN, 0},{W3_PORT, W3_PIN, 1}},
    {{W0_PORT, W0_PIN, 0},{W1_PORT, W1_PIN, 0},{W2_PORT, W2_PIN, 1},{W3_PORT, W3_PIN, 1}},
    {{W0_PORT, W0_PIN, 0},{W1_PORT, W1_PIN, 0},{W2_PORT, W2_PIN, 1},{W3_PORT, W3_PIN, 0}},
    {{W0_PORT, W0_PIN, 0},{W1_PORT, W1_PIN, 1},{W2_PORT, W2_PIN, 1},{W3_PORT, W3_PIN, 0}},
    {{W0_PORT, W0_PIN, 0},{W1_PORT, W1_PIN, 1},{W2_PORT, W2_PIN, 0},{W3_PORT, W3_PIN, 0}},
    {{W0_PORT, W0_PIN, 1},{W1_PORT, W1_PIN, 1},{W2_PORT, W2_PIN, 0},{W3_PORT, W3_PIN, 0}},
    {{W0_PORT, W0_PIN, 1},{W1_PORT, W1_PIN, 0},{W2_PORT, W2_PIN, 0},{W3_PORT, W3_PIN, 0}},
    {{W0_PORT, W0_PIN, 1},{W1_PORT, W1_PIN, 0},{W2_PORT, W2_PIN, 0},{W3_PORT, W3_PIN, 1}},  
};

static void setStep(int num) {
    if (num < STEPS) {
        for (size_t i=0;i<4;i++) {
            HAL_GPIO_WritePin(state[num][i].gpio, state[num][i].pin, 
            state[num][i].state);
        }
    }
}

void step_init(stepCtx_t* pCtx, uint32_t stepsPerTurn, float timerFreq) 
{
    pCtx->_timerFreq = timerFreq;
    pCtx->_stepsPerTurn = stepsPerTurn;
    pCtx->_steps = 0;
    pCtx->_lastTimerVal = 0;
    pCtx->_timerValPerStep = 0;
    pCtx->_stepIndex = 0;
}

void step_cw(stepCtx_t *pCtx, float speedHz, uint32_t steps) {
    if (pCtx) {
        pCtx->_steps = steps;
        pCtx->_dir = CW;
        pCtx->_lastTimerVal = 0;
        pCtx->_timerValPerStep = pCtx->_timerFreq/(speedHz*pCtx->_stepsPerTurn); 
    }
}

void step_ccw(stepCtx_t *pCtx, float speedHz, uint32_t steps) {
if (pCtx) {
        pCtx->_steps = steps;
        pCtx->_dir = CCW;
        pCtx->_lastTimerVal = 0;
        pCtx->_timerValPerStep = pCtx->_timerFreq/(speedHz*pCtx->_stepsPerTurn); 
    }
}

void step_func(stepCtx_t *pCtx, uint32_t timerVal) {
    if (pCtx) {
        if (pCtx->_steps) {
           if(pCtx->_lastTimerVal == 0) {
                pCtx->_lastTimerVal = timerVal; //update TimerVal after start
           } else {
                if (timerVal > (pCtx->_lastTimerVal + pCtx->_timerValPerStep)) {
                   pCtx->_lastTimerVal = timerVal + pCtx->_timerValPerStep;
                   setStep(pCtx->_stepIndex);
                   if(pCtx->_dir == CCW) {
                       pCtx->_stepIndex = pCtx->_stepIndex == 7 ? 
                        0 : pCtx->_stepIndex + 1;
                   } else {
                       pCtx->_stepIndex = pCtx->_stepIndex == 0 ? 
                        7 : pCtx->_stepIndex - 1;
                   }
                   pCtx->_steps -= 1;
               }
           }
        }
    }
}

bool step_isRunning(stepCtx_t *pCtx) {
    return pCtx && pCtx->_steps ? true : false;
}

