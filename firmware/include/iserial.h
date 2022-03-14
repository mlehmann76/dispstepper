#ifndef ISERIAL_H
#define ISERIAL_H

#include <string>

class iSerial {
public:
  virtual void read(char *buf, const size_t maxLen, size_t &len) = 0;
  virtual void write(const std::string_view &s) = 0;
  virtual void write(const char *buf, const size_t len) = 0;

  virtual ~iSerial() {};
};

#endif // !ISERIAL_H_
