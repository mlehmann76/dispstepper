#ifndef USB_CDC_MAIN_H
#define USB_CDC_MAIN_H

#include "inplace_function.h"
#include <string>
#include "iserial.h"

/**
 * \berif Initialize USB
 */
extern "C" {
void usb_init(void);
}

class usb_cdc_wrapper : public iSerial {
  using readcd_type = stdext::inplace_function<void(char)>;

public:
  usb_cdc_wrapper() {}
  usb_cdc_wrapper(const usb_cdc_wrapper &) = delete;
  usb_cdc_wrapper(usb_cdc_wrapper&&) = delete;
  //
  void operator=(const usb_cdc_wrapper &) = delete;
  void operator=(usb_cdc_wrapper&&) = delete;
  //
  void read(char *buf, const size_t maxLen, size_t &len) override;
  void write(const std::string_view &s) override;
  void write(const char *buf, const size_t len) override;

private:
  size_t m_readlen;
};

#endif // USB_CDC_MAIN_H