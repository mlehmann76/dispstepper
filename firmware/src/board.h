#ifndef __BOARD_H_

#include <hal_gpio.h>

void SysTick_Init();
uint32_t getTick();

template <typename TGpio, TGpio... Gpio> class keycheck {
  struct check_t {
    check_t(TGpio _g) : _gpio(_g), _state(false), _count(0) {}
    TGpio _gpio;
    bool _state;
    uint32_t _count;
  };

public:
  constexpr keycheck() : m_gpio{Gpio...} {}

  void run() {
    for (auto key : m_gpio) {
      if (gpio_get_pin_level(key._gpio) != key._state) {
        key._count = 0;
        key._state = gpio_get_pin_level(key._gpio);
      }
    }
  }

  template <TGpio gpio> constexpr bool state() const {
    for (auto key : m_gpio) {
      if (key._gpio == gpio) {
        return key._state;
      }
    }
    return true; // TODO return invalid if not found
  }

  template <TGpio gpio> constexpr bool pressed() const {
    for (auto key : m_gpio) {
      if (key._gpio == gpio) {
        return state<gpio>() == false && key._count > 50;
      }
    }
    return false;
  }

private:
  const check_t m_gpio[sizeof...(Gpio)];
};

#endif // !__BOARD_H_
