#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>

static constexpr int MODES = 3;
static constexpr int MAXLEVEL = 3;
static constexpr int TIMEOUT = 3000;

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
  template <int Size> struct collection {
    uint32_t len = Size;
    union {
      int32_t i32;
      uint32_t u32;
      float f32;
    } data[Size] = {{0}, {0}, {0}, {0}, {0}, {0}, {0},
                    {0}, {0}, {0}, {0}, {0}, {0}};
    uint32_t crc = 0;
  };
  //
  Config();
  Config(const Config&) = delete;
  Config(Config&&) = delete;
  Config& operator=(const Config&) = delete;
  //
  template <typename T> T get(index_e i) const;
  //
  index_e toIndex(uint32_t idx) {
    return idx < IDX_Last ? static_cast<index_e>(idx) : IDX_Last;
  }
private:
  collection<IDX_Last> m_coll;
};

template <> inline int32_t Config::get(Config::index_e i) const {
  return m_coll.data[i].i32;
}
template <> inline uint32_t Config::get(Config::index_e i) const {
  return m_coll.data[i].u32;
}
template <> inline viewMode Config::get(Config::index_e i) const {
  return static_cast<viewMode>(m_coll.data[i].u32);
}
template <> inline float Config::get(Config::index_e i) const {
  return m_coll.data[i].f32;
}

#endif // !__CONFIG_H__
