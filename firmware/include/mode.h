#ifndef __MODE_H_
#define __MODE_H_

#include "board.h"
#include "leds.h"
#include "user_board.h"
#include <functional>

static constexpr int MODES = 3;
static constexpr int MAXLEVEL = 3;
static constexpr int TIMEOUT = 3000;

enum viewMode { ModeSingle = 0, ModeRepeat = 1, ModeManual = 2 };

template <typename TGpio, typename buttonCheckType, typename ledViewType>
class mode {
public:
  using modeCbType = std::function<void(viewMode, uint32_t)>;
  using buttonCbType = std::function<void(TGpio, bool)>;

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
        m_modeIndex() {}

  void run(uint32_t tick) {
    //
    m_button.run(tick);
    //
    if (m_modeSelect.isEnabled) {
      onModeButton(tick);
      onUpDownButton(tick);
      if (m_modeSelect.isTimeOut(tick)) {
        m_modeSelect.isEnabled = false;
        m_led.set(m_modeIndex[m_mode], bar{});
        if (m_modeChangeCb) {
          m_modeChangeCb(m_mode, modeValue());
        }
      }
    } else if (m_button.stateChanged(SW_MODE) && m_button.pressed(SW_MODE)) {
      m_modeSelect.select(tick + TIMEOUT);
      m_led.set(m_mode, single{});
    } else {
      checkbuttons();
    }
  }

  void checkbuttons() {
    constexpr static TGpio buttons[] = {SW_UP, SW_DOWN, SW_SEL};
    if (m_buttonChangeCb) {
      for (auto b : buttons) {
        if (m_button.stateChanged(b)) {
          m_buttonChangeCb(b, m_button.state(b) == buttonCheckType::bpressed);
        }
      }
    }
  }

  void registerModeCb(modeCbType &&cb) { m_modeChangeCb = cb; }
  void registerButtonCb(buttonCbType &&cb) { m_buttonChangeCb = cb; }

  uint32_t modeValue() const { return m_modeIndex[m_mode]; }

private:
  void onModeButton(uint32_t tick) {
    if (m_button.stateChanged(SW_MODE) && m_button.pressed(SW_MODE)) {
      switch (m_mode) {
      case ModeSingle:
        m_mode = ModeRepeat;
        break;
      case ModeRepeat:
        m_mode = ModeManual;
        break;
      case ModeManual:
        m_mode = ModeSingle;
        break;
      };
      m_led.set(m_mode, single{});
      m_modeSelect.select(tick + TIMEOUT);
    }
  }
  //
  void onUpDownButton(uint32_t tick) {
    if (m_button.stateChanged(SW_UP) && m_button.pressed(SW_UP) &&
        m_modeIndex[m_mode] < MAXLEVEL) {
      m_modeIndex[m_mode]++;
      m_led.set(m_modeIndex[m_mode], bar{});
      m_modeSelect.select(tick + TIMEOUT);
    }
    if (m_button.stateChanged(SW_DOWN) && m_button.pressed(SW_DOWN) &&
        m_modeIndex[m_mode] > 0) {
      m_modeIndex[m_mode]--;
      m_led.set(m_modeIndex[m_mode], bar{});
      m_modeSelect.select(tick + TIMEOUT);
    }
  }

  buttonCheckType m_button;
  ledViewType m_led;
  viewMode m_mode;
  modeselect m_modeSelect;
  uint32_t m_modeIndex[MODES];
  modeCbType m_modeChangeCb;
  buttonCbType m_buttonChangeCb;
};

#endif // __MODE_H_