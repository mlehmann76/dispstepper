#ifndef __MODE_H_
#define __MODE_H_

#include "user_board.h"
#include "board.h"
#include "leds.h"
#include <functional>

static constexpr int MODES = 2;
enum viewMode { ModeSingle = 0, ModeRepeat = 1 };

template <typename TGpio, typename buttonCheckType, typename ledViewType>
class mode {

  struct modeselect {
    void select(uint32_t tick) {
      isEnabled = true;
      count = tick;
    }
    bool isTimeOut(uint32_t tick) const { return tick > count; }
    //
    bool isEnabled;
    uint32_t count;
  };

public:

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
        m_modeSelect.select(tick + 1000); // TODO define Ticktime
        if (m_modeChangeCb) {
          m_modeChangeCb(m_mode);
        }
      }
      if (m_modeSelect.isTimeOut(tick)) {
        m_modeSelect.isEnabled = false;
      }
    } else {
      if (m_button.pressed(SW_MODE)) {
        m_modeSelect.select(tick + 1000); // TODO define Ticktime
      }
      m_led.set(m_modeValues[m_mode], bar{});
    }

    checkbuttons();
  }

  void checkbuttons() {
    constexpr static TGpio buttons[] = {SW_UP, SW_DOWN, SW_SEL};
    if (m_buttonChangeCb) {
      for (auto b : buttons) {
        if (m_button.stateChanged(b)) {
          m_buttonChangeCb(b, m_button.state(b));
        }
      }
    }
  }

  void registerCb(std::function<void(viewMode)> &&cb) { m_modeChangeCb = cb; }
  void registerCb(std::function<void(TGpio, bool)> &&cb) {
    m_buttonChangeCb = cb;
  }

private:
  buttonCheckType m_button;
  ledViewType m_led;
  viewMode m_mode;
  modeselect m_modeSelect;
  uint32_t m_modeValues[MODES];
  std::function<void(viewMode)> m_modeChangeCb;
  std::function<void(TGpio, bool)> m_buttonChangeCb;
};

#endif // __MODE_H_