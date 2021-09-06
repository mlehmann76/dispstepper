#ifndef __LED_H_
#define __LED_H_

#include <functional> //std::function
#include "board.h"

struct single {};
struct multi {};
struct bar {};

template <typename TGpio, TGpio... gpios> class ledview {
  constexpr size_t size() { return sizeof...(gpios); }

public:
  constexpr ledview() : m_gpio{gpios...} {
    for (auto l : m_gpio) {
      wrap_gpio_set_pin_level(l, 0);
    }
  }

  constexpr void set(TGpio val, std::function<bool(TGpio, size_t)> func) {
    for (size_t i = 0; i < size(); i++) {
      if (func(val, i)) {
        wrap_gpio_set_pin_level(m_gpio[i], 0);
      } else {
        wrap_gpio_set_pin_level(m_gpio[i], 1);
      }
    }
  }

  constexpr void set(TGpio val, single) {
    set(val, [](TGpio v, size_t i) { return v == i; });
  }

  constexpr void set(TGpio val, multi) {
    set(val, [](TGpio v, size_t i) { return v == (1ul << i); });
  }

  constexpr void set(TGpio val, bar) {
    set(val, [](TGpio v, size_t i) { return v >= i; });
  }

private:
  const TGpio m_gpio[sizeof...(gpios)];
};
#endif // !__LED_H_
