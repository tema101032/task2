#include "init.h"

//volatile  port_t *myPortC  = (volatile  port_t *)(GPIOC_BASE + 0x0C);
//указатель на void, которому присваивается адрес регистра ODR порта GPIOA
void *GPIOC_ODR_ADDRESS = (void *) (GPIOC_BASE + 0x0C);
//макрос для обращения через точку к битовым полям структуры port_t
#define MYPORTC (*(port_t *) GPIOC_ODR_ADDRESS)

//переменная для результата усреднения значений измеренных ADC1
uint16_t gAdcAvrResult;

/**
 * @brief Обработчик прерывания TIM6
 * 
 */
void TIM6_DAC_IRQHandler()
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    MYPORTC.B5 ^= 1U;//для изменения состояния порта с частотой 300 Гц
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  }
}

/**
 * @brief Обработчик прерывания TIM7
 * 
 */
void TIM7_IRQHandler()
{
  static uint8_t timer7Counter = 0;
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
    MYPORTC.B6 ^= 1U;//для изменения состояния порта с частотой 10 кГц
    timer7Counter++;
    if (timer7Counter == 0x0A){ //для изменения состояния порта с частотой 1 кГц
       MYPORTC.B7 ^= 1U;
       timer7Counter = 0;
    }
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  }
}

/**
 * @brief Обработчик прерывания ADC1, в котором забираются данные с ADC1
 *  и усредняются через каждые 32 измерения
 * 
 */
void ADC1_IRQHandler(void) {
    static uint8_t adcCounter = 0;
    static uint32_t adcSum = 0;
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
        adcSum += ADC_GetConversionValue(ADC1);
        adcCounter++;
        if (adcCounter >= ADC_NUMBER_OF_VALUES){
            gAdcAvrResult = adcSum >> 5; //деление на 32
            adcCounter = 0;
            adcSum = 0;
        }
    };
};
