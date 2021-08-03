/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or
 * main.c to avoid loosing it when reconfiguring.
 */
#include "usb_cdc.h"
#include "atmel_start.h"

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
volatile bool cdcTransferRead = false;
volatile uint32_t cdcTransferReadLen;
volatile bool cdcTransferWrite = false;
volatile bool cdcConnected = false;

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
  if (cdcdf_acm_read((uint8_t *)buf, length) != USB_OK) {
    cdcTransferRead = false;
    return 0;
  }
  while (cdcTransferRead && cdcConnected)
    ;
  return (int32_t)cdcTransferReadLen;
}

static uint8_t outBuf[80];
static uint32_t outLen = 0;

static int32_t cdcWrite(const char *const buf, const uint16_t length) {
  const char *end = buf + length;
  for (const char *p = buf; p < end && cdcConnected; ++p) {
    outBuf[outLen++] = *p;

    if (*p == '\n' || outLen == sizeof(outBuf)) {
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
void cdc_device_acm_init(void) {
  /* usb stack init */
  usbdc_init(ctrl_buffer);

  /* usbdc_register_funcion inside */
  cdcdf_acm_init();

  usbdc_start(single_desc);
  usbdc_attach();
}

/**
 * Example of using CDC ACM Function.
 * \note
 * In this example, we will use a PC as a USB host:
 * - Connect the DEBUG USB on XPLAINED board to PC for program download.
 * - Connect the TARGET USB on XPLAINED board to PC for running program.
 * The application will behave as a virtual COM.
 * - Open a HyperTerminal or other COM tools in PC side.
 * - Send out a character or string and it will echo the content received.
 */
void cdcd_acm_example(void) {
  while (!cdcdf_acm_is_enabled()) {
    // wait cdc acm to be installed
  };

  cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C,
                              (FUNC_PTR)usb_device_cb_state_c);
}

void usb_service(char *ret, size_t *len) {
  if (len && ret) {
    *len = 0;
    if (cdcConnected) {
      char input;
      if (cdcRead(&input, 1) == 1) {
        cdcWrite("Got: ", 5);
        cdcWrite(&input, 1);
        cdcWrite("\r\n", 2);
        *len = 1;
        *ret = input;
      }
    }
  }
}

void usb_init(void) { cdc_device_acm_init(); }
