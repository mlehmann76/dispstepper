#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "mode.h"
class stepCtrl;

class control {
  typedef enum { SIDLE, SDIRCW, SDIRCCW, SRUN, SRETRACT } state_t;

public:
  control(stepCtrl *_s);
  //
  void run();
  //
  void onModeChange(viewMode, uint32_t);
  template <typename TGpio> void onButtonChange(TGpio, bool);

private:
  void onModeSingleRepeat();
  void onModeManual();
  float modeValue() const {
    return 0.2; //FIXME
  }

  stepCtrl *m_stepper;
  state_t sstate;
  viewMode m_mode;
  uint8_t m_lastButton;
  bool m_lastButtonState;
  uint32_t m_modeValue;
};
#endif // !__CONTROL_H_