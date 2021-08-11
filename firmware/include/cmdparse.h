#ifndef _COMMANDPARSE_MAIN
#define _COMMANDPARSE_MAIN

#include "simpleparser.h"
#include "config.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <errno.h>
#include <optional>

class Config;
class usb_cdc_wrapper;

class CmdParse {
  static constexpr std::array<char, 2> crlf = {'\r', '\n'};
  using link_type = simpleparser::KeywordPatternLink<char, 4>;

public:
  CmdParse(Config &_c, usb_cdc_wrapper &u);
  void push(char c);

  void service();
  Config &config() const;
  usb_cdc_wrapper &cdc() const;
  //
  void send(float f);
  void send(uint32_t u32);
  void send(int32_t u32);
  //
  bool set_f(Config::index_e, const char *buf, const size_t s);
  bool set_i(Config::index_e, const char *buf, const size_t s);
  bool set_u(Config::index_e, const char *buf, const size_t s);

private:
  void cleanup();

  static std::optional<long> strtol(const std::string_view &s, int base = 10) ;
  static std::optional<double> strtod(const std::string_view &s);

  Config &m_pconfig;
  usb_cdc_wrapper &m_cdc;
  std::array<char, 64> m_buf;
  size_t m_bufIndex;
  bool m_hasLineEnd;
  std::array<link_type, 4> m_scpi_cmd;
};

#endif // _COMMANDPARSE_MAIN
