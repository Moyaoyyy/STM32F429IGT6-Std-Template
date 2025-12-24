/**
 * @file    bsp_iic.c
 * @author  Yukikaze
 * @brief   软/硬件IIC驱动
 * @version 0.1
 * @date    2025-12-05
 * 
 * @copyright Copyright (c) 2025 Yukikaze
 * 
 */



#include "bsp_iic.h"
#include "bsp_oled.h"

GPIO_InitTypeDef gpio_initstruct;
I2C_InitTypeDef iic_initstruct;

void IIC_GPIO_Config(void)
{
    IIC_GPIO_CLK_ENABLE(IIC_GPIO_CLK, ENABLE);

#if IIC_SELECT
    /* 使能时钟 */
    IIC_CLK_ENABLE(IIC_CLK, ENABLE);

    /* 引脚映射 */
    GPIO_PinAFConfig(IIC_GPIO_PORT, GPIO_PinSource7, GPIO_AF_I2C1);
    GPIO_PinAFConfig(IIC_GPIO_PORT, GPIO_PinSource6, GPIO_AF_I2C1);

    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_OType = GPIO_OType_OD;
    gpio_initstruct.GPIO_Pin = IIC_SCL_GPIO_PIN | IIC_SDA_GPIO_PIN;
    gpio_initstruct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(IIC_GPIO_PORT, &gpio_initstruct);

    iic_initstruct.I2C_Mode = I2C_Mode_I2C;
    iic_initstruct.I2C_Ack = I2C_Ack_Enable;
    iic_initstruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    iic_initstruct.I2C_ClockSpeed = 400000;
    iic_initstruct.I2C_DutyCycle = I2C_DutyCycle_2;
    iic_initstruct.I2C_OwnAddress1 = 0;

    I2C_Init(IIC_NUM, &iic_initstruct);

    /* 使能IIC */
    I2C_Cmd(IIC_NUM, ENABLE);
#else
    /* 配置GPIO引脚
     * 此处配置GPIO为开漏模式 强下拉 弱上拉
     * 禁止所有设备输出强上拉的高电平的同时兼顾SDA输入输出
     */
    gpio_initstruct.GPIO_Mode = GPIO_Mode_OUT;
    gpio_initstruct.GPIO_OType = GPIO_OType_OD;
    gpio_initstruct.GPIO_Pin = IIC_SCL_GPIO_PIN | IIC_SDA_GPIO_PIN;
    gpio_initstruct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_Init(IIC_GPIO_PORT, &gpio_initstruct);
		
#endif
}

static void IIC_Delay(void)
{
    uint8_t i;
    for (i = 0; i < 10; i++)
        ;
}

/* IIC 通讯起始信号 */
void IIC_Start(void)
{
    /* SCL和SDA拉高 */
    IIC_SDA_1;
    IIC_SCL_1;

    IIC_Delay();
    /* SCL高电平期间SDA拉低 */
    IIC_SDA_0;

    /* 从机接收开始信号 延时 SCL拉低 准备传输数据 */
    IIC_Delay();
    IIC_SCL_0;
}

void IIC_Stop(void)
{
    /* SDA和SCL拉低 */
    IIC_SDA_0;
    IIC_SCL_0;

    /* SCL拉高 */
    IIC_SCL_1;

    /* 延时 释放SDA */
    IIC_Delay();
    IIC_SDA_1;
}

void IIC_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 从最高位按位取出byte */
        if (byte >> (7 - i) & 0x01)
            IIC_SDA_1;
        else
            IIC_SDA_0;
        /* 时钟线拉高 从机接收数据 */
        IIC_SCL_1;
        /* 延时 等待从机接收数据 */
        IIC_Delay();
        /* SCL拉低 主机发送下一位数据 */
        IIC_SCL_0;
        IIC_Delay();
    }
    /* 主机释放SDA */
    IIC_SDA_1;
    /* 延时 等待从机接收完成 */
    IIC_Delay();
}

uint8_t IIC_ReciveByte(void)
{
    uint8_t temp;

    for (uint8_t i = 0; i < 8; i++)
    {
        /* 时钟线拉高 主机接收数据 */
        IIC_SCL_1;
        IIC_Delay();
        if (IIC_SDA_READ)
            temp = IIC_SDA_READ << (7 - i);
        /* 时钟线拉低 从机继续发送数据 */
        IIC_SCL_0;
        /* 等待从机发送下一位数据 */
        IIC_Delay();
    }

    return temp;
}

/* 发送应答和不应答 */
void IIC_ACK(uint8_t ack)
{
    if (ack)
        IIC_SDA_1;
    else
        IIC_SDA_0;
    /* SCL拉高 */
    IIC_SCL_1;
    IIC_Delay();
    /* SCL拉低 */
    IIC_SCL_0;
    IIC_Delay();
    /* 主机释放总线 */
    IIC_SDA_1;
}

/* 等待应答和非应答 0:应答 1:非应答 */
uint8_t IIC_Wait_ACK(void)
{
    uint8_t ack;

    /* 主机释放SDA */
    IIC_SDA_1;
    IIC_Delay();
    /* SCL拉高 */
    IIC_SCL_1;
    IIC_Delay();
    if (IIC_SDA_READ)
        ack = 1;
    else
        ack = 0;
    /* SCL拉低 */
    IIC_SCL_0;
    IIC_Delay();

    return ack;
}
