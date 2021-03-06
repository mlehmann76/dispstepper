/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or
 * main.c to avoid loosing it when reconfiguring.
 */
#include "usb_cdc.h"
#include "atmel_start.h"
#include "cdcdf_acm.h"
#include "cdcdf_acm_desc.h"

#if CONF_USBD_HS_SP
static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    CDCD_ACM_HS_DESCES_LS_FS};
static uint8_t single_desc_bytes_hs[] = {
    /* Device descriptors and Configuration descriptors list. */
    CDCD_ACM_HS_DESCES_HS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS
#else
static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    CDCD_ACM_DESCES_LS_FS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ
#endif

static struct usbd_descriptors single_desc[] = {
    {single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
    ,
    {single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)}
#endif
};

/** Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];
static bool cdcTransferRead = false;
static uint32_t cdcTransferReadLen;
static bool cdcTransferWrite = false;
static bool cdcConnected = false;
static bool cdcInitialized = false;

static bool cdcWriteDone(const uint8_t ep, const enum usb_xfer_code rc,
                         const uint32_t count) {
  cdcTransferWrite = false;
  return false;
}

static bool cdcReadDone(const uint8_t ep, const enum usb_xfer_code rc,
                        const uint32_t count) {
  cdcTransferReadLen = count;
  cdcTransferRead = false;
  return false;
}

static int32_t cdcRead(char *const buf, const uint16_t length) {
  cdcTransferRead = true;
  int _timeout = 10000; // FIXME Magic
  if (cdcdf_acm_read((uint8_t *)buf, length) != USB_OK) {
    cdcTransferRead = false;
    return 0;
  }
  while (cdcTransferRead && cdcConnected && _timeout) {
    _timeout--;
  }
  return (int32_t)cdcTransferReadLen;
}

static uint8_t outBuf[80];
static uint32_t outLen = 0;

static int32_t cdcWrite(const char *const buf, const uint16_t length) {
  const char *end = buf + length;
  for (const char *p = buf; p < end && cdcConnected; ++p) {
    outBuf[outLen++] = *p;

    if (*p == '\n' || *p == '\r' || outLen == sizeof(outBuf)) {
      cdcTransferWrite = true;
      cdcdf_acm_write(outBuf, outLen);
      while (cdcTransferWrite && cdcConnected)
        ;
      outLen = 0;
    }
  }
  return length;
}

/**
 * \brief Callback invoked when Line State Change
 */
static bool usb_device_cb_state_c(usb_cdc_control_signal_t state) {
  if (state.rs232.DTR) {
    /* Callbacks must be registered after endpoint allocation */
    cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cdcReadDone);
    cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cdcWriteDone);
    cdcConnected = true;
  } else {
    cdcConnected = false;
  }

  /* No error. */
  return false;
}

/**
 * \brief CDC ACM Init
 */
void usb_init(void) { 
  /* usb stack init */
  usbdc_init(ctrl_buffer);

  /* usbdc_register_funcion inside */
  cdcdf_acm_init();

  usbdc_start(single_desc);
  usbdc_attach();
}

/*
 */

void usb_cdc_wrapper::read(char *buf, const size_t maxLen, size_t &len) {
  if (cdcInitialized) {
    if (buf && cdcConnected) {
      size_t l;
      if ((l = cdcRead(buf, maxLen))) {
        cdcWrite(buf, l);
        len = l;
      }
    } else {
      len = 0;
    }
  } else {
    if (cdcdf_acm_is_enabled()) {
      cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C,
                                  (FUNC_PTR)usb_device_cb_state_c);
      cdcInitialized = true;
    }
  }
}

void usb_cdc_wrapper::write(const char *buf, const size_t len) {
  cdcWrite(buf, len);
}

void usb_cdc_wrapper::write(const std::string_view &s) {
  cdcWrite(s.data(), s.size());
}
