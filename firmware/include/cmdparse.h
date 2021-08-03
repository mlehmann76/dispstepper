#ifndef _COMMANDPARSE_MAIN
#define _COMMANDPARSE_MAIN

#include "simpleparser.h"
#include <algorithm>
#include <array>

class Config;

class CmdParse {
  static constexpr std::array<char, 2> crlf = {'\r', '\n'};

public:
  CmdParse(Config *_c);
  void push(char c);

  template <typename T> void service(T &);

private:
  void cleanup();

  Config *m_pconfig;
  std::array<char, 64> m_buf;
  size_t m_bufIndex;
  bool m_hasLineEnd;
};

template <typename T> void CmdParse::service(T &cmd) {
  if (m_hasLineEnd || m_bufIndex == (m_buf.size()-1)) {
    auto p = std::find_first_of(m_buf.begin(), m_buf.end(), crlf.begin(),
                                crlf.end());
    auto diff = std::distance(m_buf.begin(), p);
    if (diff) {
      // parse
      for (auto &l : cmd) {
        l.match(std::string_view(m_buf.data(), diff)); // TODO
      }
    }
    // cleanup
    cleanup();
  }
}
#endif // _COMMANDPARSE_MAIN
