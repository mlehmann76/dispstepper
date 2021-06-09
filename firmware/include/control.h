#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "mode.h"
class stepCtrl;

class control {
  typedef enum { SIDLE, SRUN, SRETRACT } state_t;

public:
  control(stepCtrl *_s);
  //
  void run();
  //
  void onModeChange(viewMode);
  template <typename TGpio> void onButtonChange(TGpio, bool);

private:
  stepCtrl *m_stepper;
  static state_t sstate;
};
#endif // !__CONTROL_H_