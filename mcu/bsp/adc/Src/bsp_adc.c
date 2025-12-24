/**
 * @file    bsp_photoresistor.c
 * @author  Yukikaze
 * @brief   光敏模块驱动
 * @version 0.1
 * @date    2025-12-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

/**
 * @brief  光敏电阻的GPIO配置
 * @param  无
 * @retval 无
 */
static void PhotoResistor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 打开 ADC IO端口时钟
    ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);
    // 打开 数字量 IO端口时钟
    PhotoResistor_GPIO_APBxClock_FUN(PhotoResistor_GPIO_CLK, ENABLE);

    // 配置 ADC IO 引脚模式
    // 必须为模拟输入
    GPIO_InitStructure.GPIO_Pin = ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上拉不下拉
    // 初始化 ADC IO
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);

    // 配置 数字量 IO 引脚模式
    // 浮空输入
    GPIO_InitStructure.GPIO_Pin = PhotoResistor_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上拉不下拉
    // 初始化 PhotoResistor IO
    GPIO_Init(PhotoResistor_PORT, &GPIO_InitStructure);
}

/**
 * @brief  光敏电阻相关的ADC配置
 * @param  无
 * @retval 无
 */
static void PhotoResistor_ADC_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;             // ADC初始化结构定义
    ADC_CommonInitTypeDef ADC_CommonInitStructure; // ADC通用初始化结构定义
    
    // 打开ADC时钟
    ADC_APBxClock_FUN(ADC_CLK, ENABLE);

    // 独立ADC模式
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    // 时钟为fpclk x分频
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    // 禁止DMA直接访问模式
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    // 采样时间间隔
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_StructInit(&ADC_InitStructure);
    // ADC 分辨率
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    // 禁止扫描模式，多通道采集才需要
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    // 连续转换
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    // 禁止外部边沿触发
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    // 外部触发通道，本例子使用软件触发，此值随便赋值即可
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    // 数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    // 转换通道 1个
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADCx, &ADC_InitStructure);

    // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
    ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_56Cycles);
    // ADC 转换结束产生中断，在中断服务程序中读取转换值
    ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    // 使能ADC
    ADC_Cmd(ADCx, ENABLE);
    // 开始adc转换，软件触发
    ADC_SoftwareStartConv(ADCx);
}

/**
 * @brief  ADC中断配置
 * @param  无
 * @retval 无
 */
static void PhotoResistor_ADC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 优先级分组，已在BSP_Init中统一配置
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; // 抢占优先级6
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  光敏电阻初始化
 * @param  无
 * @retval 无
 */
void PhotoResistor_Init(void)
{
    PhotoResistor_GPIO_Config();
    PhotoResistor_ADC_Mode_Config();
    PhotoResistor_ADC_NVIC_Config();
}

/*********************************************END OF FILE**********************/
