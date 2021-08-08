#ifndef _COMMANDPARSE_MAIN
#define _COMMANDPARSE_MAIN

#include "simpleparser.h"
#include <algorithm>
#include <array>

class Config;
class usb_cdc_wrapper;

class CmdParse {
  static constexpr std::array<char, 2> crlf = {'\r', '\n'};
  using link_type = simpleparser::KeywordPatternLink<char, 4>;

public:
  CmdParse(Config &_c, usb_cdc_wrapper &u);
  void push(char c);

  void service();
  Config& config() const;
  usb_cdc_wrapper& cdc() const;

private:
  void cleanup();

  Config& m_pconfig;
  usb_cdc_wrapper& m_cdc;
  std::array<char, 64> m_buf;
  size_t m_bufIndex;
  bool m_hasLineEnd;
  std::array<link_type, 2> m_scpi_cmd;
};

#endif // _COMMANDPARSE_MAIN
