#ifndef __MODE_H_
#define __MODE_H_

#include "leds.h"
#include "board.h"

template <typename buttonCheckType, typename ledViewType> class mode {

  struct modeselect {
    bool isEnabled;
    uint32_t count;
  };

public:
  static constexpr int MODES = 2;
  enum viewMode { ModeSingle = 0, ModeRepeat = 1 };

  constexpr mode()
      : m_button{}, m_led{}, m_mode(ModeSingle), m_modeSelect{true, 0},
        m_modeValues() {}

  void run(uint32_t tick) {
    if (m_modeSelect.isEnabled) {
      if (m_button.pressed(SW_MODE)) {
        switch (m_mode) {
        case ModeSingle:
          m_mode = ModeRepeat;
          break;
        case ModeRepeat:
          m_mode = ModeSingle;
          break;
        };
        m_led.set(m_mode, single{});
        m_modeSelect.count = tick + 1000; // TODO define Ticktime
      }
      if (tick > m_modeSelect.count) {
        m_modeSelect.isEnabled = false;
      }
    } else {
      if (m_button.pressed(SW_MODE)) {
        m_modeSelect.isEnabled = true;
        m_modeSelect.count = tick + 1000; // TODO define Ticktime
      }
      m_led.set(m_modeValues[m_mode], bar{});
    }
  }

private:
  buttonCheckType m_button;
  ledViewType m_led;
  viewMode m_mode;
  modeselect m_modeSelect;
  uint32_t m_modeValues[MODES];
};

#endif // __MODE_H_