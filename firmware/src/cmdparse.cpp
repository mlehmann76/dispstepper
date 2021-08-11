#include "cmdparse.h"
#include "config.h"
#include "simpleparser.h"
#include "usb_cdc.h"
#include <cstdio>
#include <cstring>

CmdParse::CmdParse(Config &_c, usb_cdc_wrapper &u)
    : m_pconfig(_c), m_cdc(u), m_buf(), m_bufIndex(0), m_hasLineEnd(false),
      m_scpi_cmd{
          //
          link_type{"*IDN",
                    [this](int num) {
                      cdc().write("dispstepper_v0\r\n",
                                  strlen("dispstepper_v0\r\n"));
                    },
                    [this](std::string_view s, int num) {}},
          link_type{"SETup:SINGle#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      send(config().get<float>(
                          config().toIndex(Config::IDX_ModeSingle0 + num)));
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      auto ret =
                          set_f(config().toIndex(Config::IDX_ModeSingle0 + num),
                                s.data(), s.size());
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:REPeat#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      send(config().get<float>(
                          config().toIndex(Config::IDX_ModeRepeat0 + num)));
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      auto ret =
                          set_f(config().toIndex(Config::IDX_ModeRepeat0 + num),
                                s.data(), s.size());
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:MANual#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      send(config().get<uint32_t>(
                          config().toIndex(Config::IDX_ModeManual0 + num)));
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > 3 ? 3 : num;
                      auto ret =
                          set_u(config().toIndex(Config::IDX_ModeManual0 + num),
                                s.data(), s.size());
                      cdc().write(ret ? "ack" : "nack");
                    }},
      } {}

void CmdParse::push(char c) {
  if (m_bufIndex < (m_buf.size() - 1)) {
    m_buf[m_bufIndex++] = c;
    if (c == '\r' || c == '\n') {
      m_hasLineEnd = true;
    }
  }
}

void CmdParse::cleanup() {
  m_hasLineEnd = false;
  m_bufIndex = 0;
  std::fill(m_buf.begin(), m_buf.end(), 0);
}

void CmdParse::service() {
  if (m_hasLineEnd || m_bufIndex == (m_buf.size() - 1)) {
    auto p = std::find_first_of(m_buf.begin(), m_buf.end(), crlf.begin(),
                                crlf.end());
    auto diff = std::distance(m_buf.begin(), p);
    if (diff) {
      // parse
      for (auto &cmd : m_scpi_cmd) {
        cmd.match(std::string_view(m_buf.data(), diff)); // TODO
      }
    }
    // cleanup
    cleanup();
  }
}

Config &CmdParse::config() const { return m_pconfig; }

usb_cdc_wrapper &CmdParse::cdc() const { return m_cdc; }

void CmdParse::send(float f) {
  char buf[64];
  memset(buf, 0, sizeof(buf));
  ::snprintf(buf, sizeof(buf), "%.3f", f);
  cdc().write(buf, strnlen(buf, sizeof(buf)));
}

bool CmdParse::set_f(Config::index_e i, const char *buf, const size_t s) {
  bool ret = false;
  auto d = strtod(std::string_view(buf, s));
  if (d && *d >= 0.0 && *d < 1.0) {
    config().set(i, static_cast<float>(*d));
    ret = true;
  }
  return ret;
}

bool CmdParse::set_i(Config::index_e i, const char *buf, const size_t s) {
  bool ret = false;
  auto d = strtol(std::string_view(buf, s));
  if (d && *d >= 0.0 && *d < 4096) {
    config().set(i, static_cast<int32_t>(*d));
    ret = true;
  }
  return ret;
}

bool CmdParse::set_u(Config::index_e i, const char *buf, const size_t s) {
  bool ret = false;
  auto d = strtol(std::string_view(buf, s));
  if (d && *d >= 0.0 && *d < 4096) {
    config().set(i, static_cast<uint32_t>(*d));
    ret = true;
  }
  return ret;
}

void CmdParse::send(uint32_t u32) {
  char buf[64];
  memset(buf, 0, sizeof(buf));
  ::snprintf(buf, sizeof(buf), "%lu", u32);
  cdc().write(buf, strnlen(buf, sizeof(buf)));
}

void CmdParse::send(int32_t i32) {
  char buf[64];
  memset(buf, 0, sizeof(buf));
  ::snprintf(buf, sizeof(buf), "%ld", i32);
  cdc().write(buf, strnlen(buf, sizeof(buf)));
}

std::optional<long> CmdParse::strtol(const std::string_view &s, int base) {
  errno = 0;
  const char *ptr = s.data();
  char *endptr;
  long ret = ::strtol(ptr, &endptr, base);
  if (ptr != endptr && errno == 0) {
    return {ret};
  } else {
    return {};
  }
}

std::optional<double> CmdParse::strtod(const std::string_view &s) {
  errno = 0;
  const char *ptr = s.data();
  char *endptr;
  double ret = ::strtod(ptr, &endptr);
  if (ptr != endptr && errno == 0) {
    return {ret};
  } else {
    return {};
  }
}