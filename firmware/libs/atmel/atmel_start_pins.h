/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define LED2 GPIO(GPIO_PORTA, 2)
#define LED3 GPIO(GPIO_PORTA, 3)
#define LED4 GPIO(GPIO_PORTA, 4)
#define LED5 GPIO(GPIO_PORTA, 5)
#define SW_MODE GPIO(GPIO_PORTA, 9)
#define PA10 GPIO(GPIO_PORTA, 10)
#define PA11 GPIO(GPIO_PORTA, 11)
#define SW_DONW GPIO(GPIO_PORTA, 14)
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

#endif // ATMEL_START_PINS_H_INCLUDED
