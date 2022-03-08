#include "control.h"
#include "config.h"
#include "stepper.h"

control::control(Config *_c, stepCtrl *_s)
    : m_config(_c), m_stepper(_s), sstate(SIDLE), m_lastButton(-1),
      m_lastButtonState(false), m_lastDir(DIR_NONE) {}
//
void control::run() {
  switch (mode()) {
  case ModeRepeat:
  case ModeSingle:
    onModeSingleRepeat();
    break;
  case ModeManual:
    onModeManual();
    break;
  case ModeExtern:
    onModeExtern();
  }
}
//
float control::modeValue() const {
  switch (mode()) {
  case ModeRepeat:
    switch (m_config->get<Config::IDX_ModeRepeatIdx>()) {
    case 0:
      return m_config->get<Config::IDX_ModeRepeat0>();
    case 1:
      return m_config->get<Config::IDX_ModeRepeat1>();
    case 2:
      return m_config->get<Config::IDX_ModeRepeat2>();
    case 3:
      return m_config->get<Config::IDX_ModeRepeat3>();
    }
    //
    break;
  case ModeSingle:
    switch (m_config->get<Config::IDX_ModeSingleIdx>()) {
    case 0:
      return m_config->get<Config::IDX_ModeSingle0>();
    case 1:
      return m_config->get<Config::IDX_ModeSingle1>();
    case 2:
      return m_config->get<Config::IDX_ModeSingle2>();
    case 3:
      return m_config->get<Config::IDX_ModeSingle3>();
    }
    break;
  case ModeManual:
    switch (m_config->get<Config::IDX_ModeManualIdx>()) {
    case 0:
      return m_config->get<Config::IDX_ModeManual0>();
    case 1:
      return m_config->get<Config::IDX_ModeManual1>();
    case 2:
      return m_config->get<Config::IDX_ModeManual2>();
    case 3:
      return m_config->get<Config::IDX_ModeManual3>();
    }
    break;
  case ModeExtern:
    return 0; // not handled here
  }
  return 0;
}
//
void control::onModeChange(viewMode m, uint32_t v) {
  sstate = SIDLE;
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
      m_stepper->cw(SPEED, modeValue());
      m_lastButton = -1;
      sstate = SDIRCW;
    }
    break;

  case SDIRCW:
  case SDIRCCW:
    if (m_lastButton == SW_SEL && m_lastButtonState == false) {
      m_lastButton = -1;
      sstate = SRUN;
      break;
    }
    if (mode() == ModeRepeat && !m_stepper->isRunning()) {
      m_stepper->cw(SPEED, modeValue());
    }
    break;

  case SRUN:
    if (!m_stepper->isRunning()) {
      if (m_stepper->dir() == stepCtrl::CW) {
        m_stepper->ccw(SPEED, modeValue() / 16);
      } else {
        m_stepper->cw(SPEED, modeValue() / 16);
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
//
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
    if (m_stepper->dir() == stepCtrl::CW) {
      m_stepper->ccw(modeValue(), BUTTON_STEPS / 16);
    } else {
      m_stepper->cw(modeValue(), BUTTON_STEPS / 16);
    }
    sstate = SRETRACT;
    break;

  case SRETRACT:
    if (!m_stepper->isRunning()) {
      sstate = SIDLE;
      break;
    }
  }
}
//
void control::onModeExtern() {
  if (m_config->get<Config::IDX_ModeExternDIR>() != m_lastDir) {
    m_lastDir = m_config->get<Config::IDX_ModeExternDIR>();
    switch (m_config->get<Config::IDX_ModeExternDIR>()) {
    case DIR_CCW:
      m_stepper->ccw(m_config->get<Config::IDX_ModeExternSPD>(),
                     m_config->get<Config::IDX_ModeExternSTEPS>());
      break;
    case DIR_CW:
      m_stepper->cw(m_config->get<Config::IDX_ModeExternSPD>(),
                    m_config->get<Config::IDX_ModeExternSTEPS>());
      break;
    case DIR_NONE:
      m_stepper->cw(0.0, 0);
      break;
    }
  }
  if (!m_stepper->isRunning() &&
      m_config->get<Config::IDX_ModeExternDIR>() != DIR_NONE) {
    m_config->set<Config::IDX_ModeExternDIR>(DIR_NONE);
  }
}