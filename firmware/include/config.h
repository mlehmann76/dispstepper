#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

static constexpr int MODES = 3;
static constexpr int MAXLEVEL = 3;
static constexpr int TIMEOUT = 3000;
static constexpr int BUTTON_STEPS = 256;
static constexpr float SPEED = 0.2;

enum viewMode { ModeSingle = 0, ModeRepeat = 1, ModeManual = 2 };

class Config {
  static const uint16_t ID = 1;

public:
  enum index_e {
    IDX_Mode = 0,
    IDX_ModeSingleIdx,
    IDX_ModeSingle0,
    IDX_ModeSingle1,
    IDX_ModeSingle2,
    IDX_ModeSingle3,
    IDX_ModeRepeatIdx,
    IDX_ModeRepeat0,
    IDX_ModeRepeat1,
    IDX_ModeRepeat2,
    IDX_ModeRepeat3,
    IDX_ModeManualIdx,
    IDX_ModeManual0,
    IDX_ModeManual1,
    IDX_ModeManual2,
    IDX_ModeManual3,
    IDX_Last
  };
  //
  template <index_e E> struct enumtraits {};


  template <int Size> struct collection {
    union U {
      int32_t i32;
      uint32_t u32;
      float f32;
      U(int i) : i32(i) {}
      U(uint32_t i) : u32(i) {}
      U(float i) : f32(i) {}
    };
    //
    uint32_t len = Size;
    U data[Size] = {
        0,                           //
        0, 0.05f, 0.1f, 0.15f, 0.2f, //
        0, 0.05f, 0.1f, 0.15f, 0.2f, //
        0, 16,    32,   64,    128   //
    };
    uint32_t crc = 0;
  };
  //
  Config();
  Config(const Config &) = delete;
  Config(Config &&) = delete;
  Config &operator=(const Config &) = delete;
  //
  template <index_e E> typename enumtraits<E>::value_type get() const {
    return _get<typename enumtraits<E>::value_type>(E);
  }
  //
  template <index_e E>
  void set(typename enumtraits<E>::value_type t) {
    _set<typename enumtraits<E>::value_type>(E, t);
  }
  //
  void run(uint32_t);
  //
private:
  //
  template <typename T> T _get(index_e i) const;
  //
  template <typename T> void _set(index_e i, T t);
  //
  collection<IDX_Last> m_coll;
  bool m_writeRequest;
  uint32_t m_requestTick;
};

template <> inline int32_t Config::_get(Config::index_e i) const {
  return m_coll.data[i].i32;
}
template <> inline uint32_t Config::_get(Config::index_e i) const {
  return m_coll.data[i].u32;
}
template <> inline viewMode Config::_get(Config::index_e i) const {
  return static_cast<viewMode>(m_coll.data[i].u32);
}
template <> inline float Config::_get(Config::index_e i) const {
  return m_coll.data[i].f32;
}
template <> inline void Config::_set(Config::index_e i, int32_t t) {
  m_coll.data[i].i32 = t;
  m_writeRequest = true;
}
template <> inline void Config::_set(Config::index_e i, uint32_t t) {
  m_coll.data[i].u32 = t;
  m_writeRequest = true;
}
template <> inline void Config::_set(Config::index_e i, float t) {
  m_coll.data[i].f32 = t;
  m_writeRequest = true;
}
template <> inline void Config::_set(Config::index_e i, viewMode t) {
  m_coll.data[i].u32 = t;
  m_writeRequest = true;
}
/*
*/
template <Config::index_e E> typename Config::enumtraits<E>::value_type rmin() {
  return Config::enumtraits<E>::min;
}

template <Config::index_e E> typename Config::enumtraits<E>::value_type rmax() {
  return Config::enumtraits<E>::max;
}

template <> struct Config::enumtraits<Config::index_e::IDX_Mode> {
  using value_type = viewMode;
  constexpr static value_type min = ModeSingle;
  constexpr static value_type max = ModeManual;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeSingleIdx> {
  using value_type = uint32_t;
  constexpr static value_type min = 0;
  constexpr static value_type max = 3;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeSingle0> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeSingle1> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeSingle2> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeSingle3> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeRepeatIdx> {
  using value_type = uint32_t;
  constexpr static value_type min = 0;
  constexpr static value_type max = 3;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeRepeat0> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeRepeat1> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeRepeat2> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeRepeat3> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 1.0;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeManualIdx> {
  using value_type = uint32_t;
  constexpr static value_type min = 0;
  constexpr static value_type max = 3;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeManual0> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 4096;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeManual1> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 4096;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeManual2> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 4096;
};

template <> struct Config::enumtraits<Config::index_e::IDX_ModeManual3> {
  using value_type = float;
  constexpr static value_type min = 0.0;
  constexpr static value_type max = 4096;
};

#endif // !__CONFIG_H__
