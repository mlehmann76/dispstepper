#ifndef __BOARD_H_
#define __BOARD_H_

#include <hal_gpio.h>

void SysTick_Init();
uint32_t getTick();

template <typename TGpio, TGpio... Gpio> class buttonCheck {
  enum bstate {bpressed, breleased};

  struct check_t {
    check_t(TGpio _g) : _gpio(_g), _state(breleased), _count(0) {}
    TGpio _gpio;
    bstate _state;
    uint32_t _count;
  };

public:
  constexpr buttonCheck() : m_gpio{Gpio...}, m_lasttick() {}

  void run(uint32_t tick) {
    if (m_lasttick != tick) {
      for (auto key : m_gpio) {
        bstate _gstate = gpio_get_pin_level(key._gpio) ? breleased : bpressed;
        if (_gstate != key._state) {
          key._count = 0;
          key._state = _gstate;
        }
      }
      m_lasttick = tick;
    }
  }

  template <TGpio gpio> constexpr bstate state() const {
    for (auto key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state;
      }
    }
    static_assert(true, "button unknown");
    return breleased;
  }

  template <TGpio gpio> constexpr bool pressed() const {
    for (auto key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state == bpressed && key._count > 50;
      }
    }
    static_assert(true, "button unknown"); 
    return false;
  }

  constexpr bool pressed(TGpio gpio) const {
    for (auto key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state == bpressed && key._count > 50;
      }
    }
    return false;
  }

private:
  const check_t m_gpio[sizeof...(Gpio)];
  uint32_t m_lasttick;
};

#endif // !__BOARD_H_
