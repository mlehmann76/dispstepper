#ifndef _BOARD_MAIN_H_
#define _BOARD_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GPIO(port, pin) ((((port)&0x7u) << 5) + ((pin)&0x1Fu))
#define GPIO_PORTA 0

#define LED2 GPIO(GPIO_PORTA, 2)
#define LED3 GPIO(GPIO_PORTA, 3)
#define LED4 GPIO(GPIO_PORTA, 4)
#define LED5 GPIO(GPIO_PORTA, 5)
#define SW_MODE GPIO(GPIO_PORTA, 9)
#define PA10 GPIO(GPIO_PORTA, 10)
#define PA11 GPIO(GPIO_PORTA, 11)
#define SW_DOWN GPIO(GPIO_PORTA, 14)
#define SW_SEL GPIO(GPIO_PORTA, 15)
#define SW_UP GPIO(GPIO_PORTA, 16)
#define nSLEEP GPIO(GPIO_PORTA, 17)
#define AIN1 GPIO(GPIO_PORTA, 18)
#define AIN2 GPIO(GPIO_PORTA, 19)
#define BIN2 GPIO(GPIO_PORTA, 22)
#define BIN1 GPIO(GPIO_PORTA, 23)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define nFAULT GPIO(GPIO_PORTA, 27)

void SysTick_Init(uint32_t cpuFreq);
uint32_t getTick();
void board_init();
void wrap_nv_storage_init();
int32_t wrap_nv_storage_read(const uint16_t id, const uint16_t offset,
                             uint8_t *const data, const uint16_t size);
uint16_t wrap_nv_storage_item_size(const uint16_t id);
int32_t wrap_nv_storage_write(const uint16_t id, const uint16_t offset,
                              const uint8_t *const data, const uint16_t size);
void wrap_gpio_set_pin_level(const uint8_t pin, const bool level);
bool wrap_gpio_get_pin_level(const uint8_t pin) ;
uint16_t readCounter();

#ifdef __cplusplus
}
#endif

#endif // !_BOARD_MAIN_H_
