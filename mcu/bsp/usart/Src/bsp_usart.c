/**
 * @file    bsp_usart.c
 * @author  Yukikaze
 * @brief   重定向c库printf函数到usart端口
 * @version 0.1
 * @date 2025-12-05
 *
 * @copyright Copyright (c) 2025 Yukikaze
 *
 */

#include "bsp_usart.h"

/**
 * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
void USARTx_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(USARTx_RX_GPIO_CLK | USARTx_TX_GPIO_CLK, ENABLE);

    /* 使能 USART 时钟 */
    USARTx_CLOCKCMD(USARTx_CLK, ENABLE);

    /* GPIO初始化 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* 配置Tx引脚为复用功能  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
    GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

    /* 配置Rx引脚为复用功能 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
    GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

    /* 连接 PXx 到 USARTx_Tx*/
    GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

    /*  连接 PXx 到 USARTx__Rx*/
    GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);

    /* 配置串DEBUG_USART 模式 */
    /* 波特率设置：DEBUG_USART_BAUDRATE */
    USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;
    /* 字长(数据位+校验位)：8 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /* 停止位：1个停止位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /* 校验位选择：不使用校验 */
    USART_InitStructure.USART_Parity = USART_Parity_No;
    /* 硬件流控制：不使用硬件流 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* USART模式控制：同时使能接收和发送 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* 完成USART初始化配置 */
    USART_Init(USARTx, &USART_InitStructure);

    /* 使能串口 */
    USART_Cmd(USARTx, ENABLE);
}

// 重定向底层putchar（_write会调用它）到串口发送
int __io_putchar(int ch)
{
    USART_SendData(USARTx, (uint8_t)ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
    return ch;
}

// 重定向底层getchar（_read会调用它）到串口接收
// 备注：添加超时，防止无数据时永久阻塞
int __io_getchar(void)
{
    uint32_t timeout = 800000; // 简单循环超时，约数百微秒级，避免死等
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
    {
        if (timeout-- == 0)
        {
            return -1; // 无数据可读
        }
    }
    return (int)USART_ReceiveData(USARTx);
}

/*********************************************END OF FILE**********************/
