#include "cmdparse.h"
#include "config.h"
#include "simpleparser.h"
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
          //
          link_type{"SETup:MODE",
                    [this](int num) { send(config().get<Config::IDX_Mode>()); },
                    [this](std::string_view s, int num) {
                      auto ret = set<Config::IDX_Mode>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:SINGle:INDex",
                    [this](int num) {
                      send(config().get<Config::IDX_ModeSingleIdx>());
                    },
                    [this](std::string_view s, int num) {
                      auto ret = set<Config::IDX_ModeSingleIdx>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:SINGle:VALue#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      send( //
                          num == 1   ? config().get<Config::IDX_ModeSingle1>()
                          : num == 2 ? config().get<Config::IDX_ModeSingle2>()
                          : num == 3 ? config().get<Config::IDX_ModeSingle3>()
                                     : config().get<Config::IDX_ModeSingle0>());
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      auto ret = num == 1   ? set<Config::IDX_ModeSingle1>(s)
                                 : num == 2 ? set<Config::IDX_ModeSingle2>(s)
                                 : num == 3 ? set<Config::IDX_ModeSingle3>(s)
                                            : set<Config::IDX_ModeSingle0>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:REPeat:INDex",
                    [this](int num) {
                      send(config().get<Config::IDX_ModeRepeatIdx>());
                    },
                    [this](std::string_view s, int num) {
                      auto ret = set<Config::IDX_ModeRepeatIdx>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:REPeat:VALue#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      send( //
                          num == 1   ? config().get<Config::IDX_ModeRepeat1>()
                          : num == 2 ? config().get<Config::IDX_ModeRepeat2>()
                          : num == 3 ? config().get<Config::IDX_ModeRepeat3>()
                                     : config().get<Config::IDX_ModeRepeat0>());
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      auto ret = num == 1   ? set<Config::IDX_ModeRepeat1>(s)
                                 : num == 2 ? set<Config::IDX_ModeRepeat2>(s)
                                 : num == 3 ? set<Config::IDX_ModeRepeat3>(s)
                                            : set<Config::IDX_ModeRepeat0>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:MANual:INDex",
                    [this](int num) {
                      send(config().get<Config::IDX_ModeManualIdx>());
                    },
                    [this](std::string_view s, int num) {
                      auto ret = set<Config::IDX_ModeManualIdx>(s);
                      cdc().write(ret ? "ack" : "nack");
                    }},
          link_type{"SETup:MANual:VALue#",
                    [this](int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      send( //
                          num == 1   ? config().get<Config::IDX_ModeManual1>()
                          : num == 2 ? config().get<Config::IDX_ModeManual2>()
                          : num == 3 ? config().get<Config::IDX_ModeManual3>()
                                     : config().get<Config::IDX_ModeManual0>());
                    },
                    [this](std::string_view s, int num) {
                      num = num < 0 ? 0 : num > MODES ? MODES : num;
                      auto ret = num == 1   ? set<Config::IDX_ModeManual1>(s)
                                 : num == 2 ? set<Config::IDX_ModeManual2>(s)
                                 : num == 3 ? set<Config::IDX_ModeManual3>(s)
                                            : set<Config::IDX_ModeManual0>(s);
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