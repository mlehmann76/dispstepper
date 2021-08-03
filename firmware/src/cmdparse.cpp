#include "cmdparse.h"
#include "simpleparser.h"

CmdParse::CmdParse(Config *_c)
    : m_pconfig(_c), m_buf(), m_bufIndex(0), m_hasLineEnd(false) {}

void CmdParse::push(char c) {
  if (m_bufIndex < (m_buf.size()-1)) {
    m_buf[m_bufIndex++] = c;
    if (c == '\r' || c == '\n') {
      m_hasLineEnd = true;
    }
  }
}

void CmdParse::cleanup() {
  m_hasLineEnd = false;
  m_bufIndex = 0;
  std::fill(m_buf.begin(),m_buf.end(),0);
}
