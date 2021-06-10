#include "control.h"
#include "stepper.h"

#define BUTTON_STEPS 256

control::control(stepCtrl *_s)
    : m_stepper(_s), sstate(SIDLE), m_mode(ModeSingle), m_lastButton(-1),
      m_lastButtonState(false), m_modeValue() {}
//
void control::run() {
  switch (m_mode) {
  case ModeRepeat:
  case ModeSingle:
    onModeSingleRepeat();
    break;
  default:
    onModeManual();
    break;
  }
}
//
void control::onModeChange(viewMode m, uint32_t v) {
  m_mode = m;
  sstate = SIDLE;
  m_modeValue = v;
  m_stepper->cw(0.0, 0);
}
//
template <> void control::onButtonChange(uint8_t b, bool s) {
  m_lastButton = b;
  m_lastButtonState = s;
}
//
void control::onModeSingleRepeat() {
  switch (sstate) {
  case SIDLE:
    if (m_lastButton == SW_SEL && m_lastButtonState == true) {
      m_stepper->cw(modeValue(), BUTTON_STEPS);
      m_lastButton = -1;
      sstate = SDIRCW;
    }
    break;

  case SDIRCW:
  case SDIRCCW:
  if (m_lastButton == SW_SEL && m_lastButtonState == false) {
    m_lastButton = -1; sstate = SRUN; break;
  } if (!m_stepper->isRunning()) {
    m_stepper->cw(modeValue(), BUTTON_STEPS);
  } break;

      case SRUN:
    if (!m_stepper->isRunning()) {
      if (m_stepper->dir() == stepCtrl::CW) {
        m_stepper->ccw(modeValue(), BUTTON_STEPS / 16);
      } else {
        m_stepper->cw(modeValue(), BUTTON_STEPS / 16);
      }
      sstate = SRETRACT;
    }
    break;

  case SRETRACT:
    if (!m_stepper->isRunning()) {
      sstate = SIDLE;
      break;
    }
  }
}

void control::onModeManual() {
  switch (sstate) {
  case SIDLE:
    if (m_lastButton == SW_UP && m_lastButtonState == true) {
      m_stepper->cw(modeValue(), BUTTON_STEPS);
      m_lastButton = -1;
      sstate = SDIRCW;
    } else if (m_lastButton == SW_DOWN && m_lastButtonState == true) {
      m_stepper->ccw(modeValue(), BUTTON_STEPS);
      m_lastButton = -1;
      sstate = SDIRCCW;
    }
    break;

  case SDIRCW:
    if (m_lastButton == SW_UP && m_lastButtonState == false) {
      m_lastButton = -1;
      sstate = SRUN;
      break;
    }
    if (!m_stepper->isRunning()) {
      m_stepper->cw(modeValue(), BUTTON_STEPS);
    }
    break;

  case SDIRCCW:
    if (m_lastButton == SW_DOWN && m_lastButtonState == false) {
      m_lastButton = -1;
      sstate = SRUN;
      break;
    }
    if (!m_stepper->isRunning()) {
      m_stepper->ccw(modeValue(), BUTTON_STEPS);
    }
    break;

  case SRUN:
    if (!m_stepper->isRunning()) {
      if (m_stepper->dir() == stepCtrl::CW) {
        m_stepper->ccw(modeValue(), BUTTON_STEPS / 16);
      } else {
        m_stepper->cw(modeValue(), BUTTON_STEPS / 16);
      }
      sstate = SRETRACT;
    }
    break;

  case SRETRACT:
    if (!m_stepper->isRunning()) {
      sstate = SIDLE;
      break;
    }
  }
}