#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "mode.h"

class stepCtrl;
class Config;

class control {
  typedef enum { SIDLE, SDIRCW, SDIRCCW, SRUN, SRETRACT } state_t;

public:
  control(Config *_c, stepCtrl *_s);
  //
  void run();
  //
  void onModeChange(viewMode, uint32_t);
  template <typename TGpio> void onButtonChange(TGpio, bool);

private:
  void onModeSingleRepeat();
  void onModeManual();
  void onModeExtern();
  float modeValue() const;
  viewMode mode() const {return m_config != nullptr ? m_config->get<Config::IDX_Mode>() : ModeSingle;};

  Config *m_config;
  stepCtrl *m_stepper;
  state_t sstate;
  uint8_t m_lastButton;
  bool m_lastButtonState;
  direction m_lastDir;
};
#endif // !__CONTROL_H_