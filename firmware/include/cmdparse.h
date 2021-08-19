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
    static constexpr const char *fmt();
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
  template <Config::index_e E> constexpr bool set(const std::string_view &s);

private:
  void cleanup();

  static std::optional<long> strtol(const std::string_view &s, int base = 10);
  static std::optional<double> strtod(const std::string_view &s);

  Config &m_pconfig;
  usb_cdc_wrapper &m_cdc;
  std::array<char, 64> m_buf;
  size_t m_bufIndex;
  bool m_hasLineEnd;
  std::array<link_type, 8> m_scpi_cmd;
};

template <> constexpr const char *CmdParse::typetraits<int32_t>::fmt() {
  return "%ld\r\n";
}

template <> constexpr const char *CmdParse::typetraits<uint32_t>::fmt() {
  return "%lu\r\n";
}

template <> constexpr const char *CmdParse::typetraits<float>::fmt() {
  return "%.3f\r\n";
}

template <> constexpr const char *CmdParse::typetraits<viewMode>::fmt() {
  return "%d\r\n";
}

template <typename T> void CmdParse::send(T f) {
  char buf[64];
  memset(buf, 0, sizeof(buf));
  ::snprintf(buf, sizeof(buf), CmdParse::typetraits<T>::fmt(), f);
  cdc().write(buf, strnlen(buf, sizeof(buf)));
}

template <Config::index_e E>
constexpr bool CmdParse::set(const std::string_view &s) {
  bool ret = false;
  auto d = strtod(s); //TODO use type dependent conversion
  if (d && *d >= rmin<E>() && *d <= rmax<E>()) {
    config().set<E>(static_cast<typename Config::enumtraits<E>::value_type>(*d));
    ret = true;
  }
  return ret;
}

#endif // _COMMANDPARSE_MAIN
