#ifndef _BUTTON_MAIN_H
#define _BUTTON_MAIN_H

template <typename TGpio, TGpio... Gpio> class buttonCheck {
public:
  enum bstate { bpressed, breleased };

  constexpr buttonCheck() : m_gpio{Gpio...}, m_lasttick() {}

  void run(uint32_t tick) {
    if (m_lasttick != tick) {
      for (auto& key : m_gpio) {
        bstate _gstate = wrap_gpio_get_pin_level(key._gpio) ? breleased : bpressed;
        if (_gstate != key._state) {
          key._count++;
          if (key._count == 50) {
            key._state = _gstate;
            key._stateChanged = true; 
          } 
        } else {
          key._count = 0;
        }
      }
      m_lasttick = tick;
    }
  }

  template <TGpio gpio> constexpr bstate state() const { return state(gpio); }

  constexpr bstate state(TGpio gpio) const {
    for (auto& key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state;
      }
    }
    static_assert(true, "button unknown");
    return breleased;
  }

  template <TGpio gpio> constexpr bool pressed() const { return pressed(gpio); }

  constexpr bool pressed(TGpio gpio) const {
    for (auto& key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state == bpressed && key._count >= 50;
      }
    }
    return false;
  }

  constexpr bool stateChanged(TGpio gpio) {
    bool ret = false;
    for (auto& key : m_gpio) {
      if (key._gpio == gpio) {
        ret = key._stateChanged;
        key._stateChanged = false;
        key._count = ret ? key._count+1 : key._count;
        break;
      }
    }
    return ret;
  }

private:
  struct check_t {
    check_t(TGpio _g) : _gpio(_g), _state(breleased), _count(0) {}
    TGpio _gpio;
    bstate _state;
    uint32_t _count;
    bool _stateChanged;
  };

  check_t m_gpio[sizeof...(Gpio)];
  uint32_t m_lasttick;
};


#endif //_BUTTON_MAIN_H