#ifndef __INIT_H__
#define __INIT_H__

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"

#define ADC_NUMBER_OF_VALUES (32)

/**
 * структура с битовыми полями для п.2 задания 2
 */
typedef struct{
    __IO unsigned B0 : 1;
    __IO unsigned B1 : 1;
    __IO unsigned B2 : 1;
    __IO unsigned B3 : 1;  
    __IO unsigned B4 : 1;
    __IO unsigned B5 : 1;  
    __IO unsigned B6 : 1;
    __IO unsigned B7 : 1;  
    __IO unsigned B8 : 1;
    __IO unsigned B9 : 1;  
    __IO unsigned B10 : 1;
    __IO unsigned B11 : 1;  
    __IO unsigned B12 : 1;
    __IO unsigned B13 : 1;
    __IO unsigned B14 : 1;
    __IO unsigned B15 : 1;     
}port_t;

/**
 * @brief Функция, инициализирующая периферию
 * 
 */
void initMcu(void);

#endif
