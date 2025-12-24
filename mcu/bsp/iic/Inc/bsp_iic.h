#ifndef __BSP_IIC_SOFTWARE_H
#define __BSP_IIC_SOFTWARE_H

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* IIC引脚宏定义　
 * IIC_SDA---->PB7
 * IIC_SCL---->PB6
 * 用户可自定义引脚 详见STM32官方数据手册
 */
#define IIC_NUM I2C1
#define IIC_CLK_ENABLE RCC_APB1PeriphClockCmd
#define IIC_CLK RCC_APB1Periph_I2C1
#define IIC_GPIO_PORT GPIOB
#define IIC_GPIO_CLK_ENABLE RCC_AHB1PeriphClockCmd
#define IIC_GPIO_CLK RCC_AHB1Periph_GPIOB
#define IIC_SDA_GPIO_PIN GPIO_Pin_7
#define IIC_SCL_GPIO_PIN GPIO_Pin_6

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#define IIC_SDA_0 GPIO_ResetBits(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN)
#define IIC_SDA_1 GPIO_SetBits(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN)
#define IIC_SCL_0 GPIO_ResetBits(IIC_GPIO_PORT, IIC_SCL_GPIO_PIN)
#define IIC_SCL_1 GPIO_SetBits(IIC_GPIO_PORT, IIC_SCL_GPIO_PIN)

#define IIC_SDA_READ GPIO_ReadInputDataBit(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN)

void IIC_GPIO_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_ACK(uint8_t ack);
uint8_t IIC_Wait_ACK(void);
void IIC_SendByte(uint8_t byte);
uint8_t IIC_ReciveByte(void);

#endif
