#ifndef USB_CDC_MAIN_H
#define USB_CDC_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "cdcdf_acm.h"
#include "cdcdf_acm_desc.h"

void cdcd_acm_example(void);
void cdc_device_acm_init(void);
void usb_service();

/**
 * \berif Initialize USB
 */
void usb_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USB_CDC_MAIN_H