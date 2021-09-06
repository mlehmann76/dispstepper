#ifndef __MODE_H_
#define __MODE_H_

#include "board.h"
#include "config.h"
#include "inplace_function.h"
#include "leds.h"
//#include "user_board.h"
#include <cstdint>

template <typename TGpio, typename buttonCheckType, typename ledViewType>
class mode {
public:
  using modeCbType = stdext::inplace_function<void(viewMode, uint32_t)>;
  using buttonCbType = stdext::inplace_function<void(TGpio, bool)>;

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
  constexpr mode(Config *_c)
      : m_button{}, m_led{}, m_mode(_c->get<Config::IDX_Mode>()),
        m_config(_c), m_modeSelect{true, 0},
        m_modeIndex{_c->get<Config::IDX_ModeSingleIdx>(),
                    _c->get<Config::IDX_ModeRepeatIdx>(),
                    _c->get<Config::IDX_ModeManualIdx>()} {}
  //
  void run(uint32_t tick);
  //
  void checkbuttons();
  //
  void registerModeCb(modeCbType &&cb) { m_modeChangeCb = cb; }
  void registerButtonCb(buttonCbType &&cb) { m_buttonChangeCb = cb; }
  //
  uint32_t modeValue() const { return m_modeIndex[m_mode]; }

private:
  void onModeButton(uint32_t tick);
  //
  void onUpDownButton(uint32_t tick);
  //
  void checkConfig(uint32_t tick);
  //
  void updateLed() {
    if (m_modeSelect.isEnabled) {
      m_led.set(m_mode, single{});
    } else {
      m_led.set(m_modeIndex[m_mode], bar{});
    }
  }

  buttonCheckType m_button;
  ledViewType m_led;
  viewMode m_mode;
  Config *m_config;
  modeselect m_modeSelect;
  uint32_t m_modeIndex[MODES];
  modeCbType m_modeChangeCb;
  buttonCbType m_buttonChangeCb;
};
/*
*
*/
template <typename TGpio, typename buttonCheckType, typename ledViewType>
void mode<TGpio, buttonCheckType, ledViewType>::run(uint32_t tick) {
  //
  m_button.run(tick);
  //
  checkConfig(tick);
  //
  if (m_modeSelect.isEnabled) {
    onModeButton(tick);
    onUpDownButton(tick);
    if (m_modeSelect.isTimeOut(tick)) {
      m_modeSelect.isEnabled = false;
      updateLed();
      if (m_modeChangeCb) {
        m_modeChangeCb(m_mode, modeValue());
      }
    }
  } else if (m_button.stateChanged(SW_MODE) && m_button.pressed(SW_MODE)) {
    m_modeSelect.select(tick + TIMEOUT);
    updateLed();
  } else {
    checkbuttons();
  }
}
/*
 *
 */
template <typename TGpio, typename buttonCheckType, typename ledViewType>
void mode<TGpio, buttonCheckType, ledViewType>::checkbuttons() {
  constexpr static TGpio buttons[] = {SW_UP, SW_DOWN, SW_SEL};
  if (m_buttonChangeCb) {
    for (auto b : buttons) {
      if (m_button.stateChanged(b)) {
        m_buttonChangeCb(b, m_button.state(b) == buttonCheckType::bpressed);
      }
    }
  }
}
/*
 *
 */
template <typename TGpio, typename buttonCheckType, typename ledViewType>
void mode<TGpio, buttonCheckType, ledViewType>::onModeButton(uint32_t tick) {
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
    m_modeSelect.select(tick + TIMEOUT);
    updateLed();
  }
}
/*
 *
 */
template <typename TGpio, typename buttonCheckType, typename ledViewType>
void mode<TGpio, buttonCheckType, ledViewType>::onUpDownButton(uint32_t tick) {
  if (m_button.stateChanged(SW_UP) && m_button.pressed(SW_UP) &&
      m_modeIndex[m_mode] < MAXLEVEL) {
    m_modeIndex[m_mode]++;
    updateLed();
    m_modeSelect.select(tick + TIMEOUT);
  }
  if (m_button.stateChanged(SW_DOWN) && m_button.pressed(SW_DOWN) &&
      m_modeIndex[m_mode] > 0) {
    m_modeIndex[m_mode]--;
    updateLed();
    m_modeSelect.select(tick + TIMEOUT);
  }
}
/*
 *
 */
template <typename TGpio, typename buttonCheckType, typename ledViewType>
void mode<TGpio, buttonCheckType, ledViewType>::checkConfig(uint32_t tick) {
  bool update = false;
  if (m_config->get<Config::IDX_Mode>() != m_mode) {
    m_mode = m_config->get<Config::IDX_Mode>();
    m_modeSelect.select(tick + TIMEOUT);
    update = true;
  }
  if (m_modeIndex[ModeSingle] != m_config->get<Config::IDX_ModeSingleIdx>()) {
    m_modeIndex[ModeSingle] = m_config->get<Config::IDX_ModeSingleIdx>();
    update = true;
  }
  if (m_modeIndex[ModeRepeat] != m_config->get<Config::IDX_ModeRepeatIdx>()) {
    m_modeIndex[ModeRepeat] = m_config->get<Config::IDX_ModeRepeatIdx>();
    update = true;
  }
  if (m_modeIndex[ModeManual] != m_config->get<Config::IDX_ModeManualIdx>()) {
    m_modeIndex[ModeManual] = m_config->get<Config::IDX_ModeManualIdx>();
    update = true;
  }
  if (update) {
    updateLed();
  }
}
#endif // __MODE_H_