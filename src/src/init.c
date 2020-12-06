#include "init.h"

static void initGpio(void);
static void initAdc1(void);
static void initTimer3(void);
static void initTimer6(void);
static void initTimer7(void);

/**
 * @brief Функция, инициализирующая периферию
 * 
 */
void initMcu(){
    initGpio();
    initAdc1();
    initTimer3();
    initTimer6();
    initTimer7();
}

/**
 * @brief Функция, инициализирующая GPIO
 * 
 */
static void initGpio(){
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    //GPIOC для изменений состояний порта
    GPIO_DeInit(GPIOC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    //ADC1_GPIO
    GPIO_DeInit(GPIOA);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief Функция, инициализирующая ADC1
 * 
 */
static void initAdc1(void){
    ADC_InitTypeDef ADC_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; //выбор триггера для запуска преобразования
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_41Cycles5);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); //разрешение прерываний по окончанию преобразований
    
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
    
    NVIC_EnableIRQ(ADC1_IRQn);
} 

/**
 * @brief Функция, инициализирующая TIM3
 * 
 */
static void initTimer3(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_Prescaler     = 240 - 1; //тактовая частота 24 МГц 
    TIM_TimeBaseInitStruct.TIM_Period        = 1000; // 24 000 000 / 240 / 1000 = 100 Гц
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); 
    
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); //АЦП запускается по триггеру из TIM3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief Функция, инициализирующая TIM6 для изменения состояния порта
 *  с частотой 300 Гц
 * 
 */
static void initTimer6(){
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_Prescaler = 80 - 1; //тактовая частота 24 МГц 
    TIM_InitStruct.TIM_Period = 1000; // 24 000 000 / 80 / 1000 = 300 Гц
    TIM_TimeBaseInit(TIM6, &TIM_InitStruct);   
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM6, ENABLE);
    
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

/**
 * @brief Функция, инициализирующая TIM7 для изменения состояния порта
 *  с частотой 10 кГц и 1 кГц
 * 
 */
static void initTimer7(){
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    TIM_DeInit(TIM7);
    TIM_TimeBaseStructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_Prescaler = 24 - 1; //тактовая частота 24 МГц 
    TIM_InitStruct.TIM_Period = 100; // 24 000 000 / 24 / 100 = 10 кГц
    TIM_TimeBaseInit(TIM7, &TIM_InitStruct);   
    
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
    
    NVIC_EnableIRQ(TIM7_IRQn);
}
