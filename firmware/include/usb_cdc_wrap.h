#ifndef USB_CDC_MAIN_H
#define USB_CDC_MAIN_H

#include "inplace_function.h"
#include <string>

/**
 * \berif Initialize USB
 */
extern "C" {
void usb_init(void);
void usb_cdc_service(char *ret, const size_t maxLen, size_t *len);
int32_t cdcWrite(const char *const buf, const uint16_t length);
}

class usb_cdc_wrapper {
  using readcd_type = stdext::inplace_function<void(char)>;

public:
  usb_cdc_wrapper() : m_readcb{}, m_readlen(0) {}
  usb_cdc_wrapper(const usb_cdc_wrapper &) = delete;
  usb_cdc_wrapper(usb_cdc_wrapper&&) = delete;
  usb_cdc_wrapper(readcd_type rcb) : m_readcb{rcb}, m_readlen(0) {}
  //
  void operator=(const usb_cdc_wrapper &) = delete;
  void operator=(usb_cdc_wrapper&&) = delete;
  //
  void read() {
    usb_cdc_service(m_buf, sizeof(m_buf), &m_readlen);
    if (m_readlen && m_readcb) {
      for (size_t i = 0; i < m_readlen; i++) {
        m_readcb(m_buf[i]);
      }
    }
  }
  void write(const std::string_view &s) { cdcWrite(s.data(), s.size()); }
  void write(const char *buf, const size_t len) { cdcWrite(buf, len); }
  void set(readcd_type &&rcb) { m_readcb = rcb; }

private:
  readcd_type m_readcb;
  char m_buf[64];
  size_t m_readlen;
};

#endif // USB_CDC_MAIN_H