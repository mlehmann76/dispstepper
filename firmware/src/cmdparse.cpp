#include "cmdparse.h"
#include "simpleparser.h"
#include "usb_cdc.h"
#include <cstring>

CmdParse::CmdParse(Config &_c, usb_cdc_wrapper &u)
    : m_pconfig(_c), m_cdc(u), m_buf(), m_bufIndex(0), m_hasLineEnd(false),
      m_scpi_cmd{//
                 link_type{"*IDN",
                           [this]() {
                             cdc().write("dispstepper_v0\r\n",
                                         strlen("dispstepper_v0\r\n"));
                           },
                           [this](std::string_view s) {}},
                 link_type{"TEST",
                           [this]() {
                             cdc().write("test success\r\n",
                                         strlen("test success\r\n"));
                           },
                           [this](std::string_view s) {}}} {}

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
