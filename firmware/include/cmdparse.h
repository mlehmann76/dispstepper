#ifndef _COMMANDPARSE_MAIN
#define _COMMANDPARSE_MAIN

#include "config.h"
#include "simpleparser.h"
#include "usb_cdc.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <optional>

class Config;
class usb_cdc_wrapper;

class CmdParse {
  static constexpr std::array<char, 2> crlf = {'\r', '\n'};
  using link_type = simpleparser::KeywordPatternLink<char, 4>;

  template <typename T> struct typetraits {
    static constexpr const char *fmt() { return ""; }
  };

public:
  CmdParse(Config &_c, usb_cdc_wrapper &u);
  void push(char c);

  void service();
  Config &config() const;
  usb_cdc_wrapper &cdc() const;
  //
  template <typename T> void send(T f);
  //
  template <typename T>
  bool set(Config::index_e, const std::string_view &s, T min, T max);

private:
  void cleanup();

  static std::optional<long> strtol(const std::string_view &s, int base = 10);
  static std::optional<double> strtod(const std::string_view &s);

  Config &m_pconfig;
  usb_cdc_wrapper &m_cdc;
  std::array<char, 64> m_buf;
  size_t m_bufIndex;
  bool m_hasLineEnd;
  std::array<link_type, 4> m_scpi_cmd;
};

template <> constexpr const char *CmdParse::typetraits<int32_t>::fmt() {
  return "%ld";
}

template <> constexpr const char *CmdParse::typetraits<uint32_t>::fmt() {
  return "%lu";
}

template <> constexpr const char *CmdParse::typetraits<float>::fmt() {
  return "%.3f";
}

template <typename T> void CmdParse::send(T f) {
  char buf[64];
  memset(buf, 0, sizeof(buf));
  ::snprintf(buf, sizeof(buf), CmdParse::typetraits<T>::fmt(), f);
  cdc().write(buf, strnlen(buf, sizeof(buf)));
}

template <typename T>
bool CmdParse::set(Config::index_e i, const std::string_view &s, T min, T max) {
  bool ret = false;
  auto d = strtod(s);
  if (d && *d >= min && *d < max) {
    config().set(i, static_cast<T>(*d));
    ret = true;
  }
  return ret;
}


#endif // _COMMANDPARSE_MAIN
