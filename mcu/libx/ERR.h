/**
 * @file		错误定义与处理头文件
 * @brief		用于错误类型的定义与处理函数的声明
 * @author	王广平
 * @date		2025/11/5
 */
#ifndef __ERR_H
#define __ERR_H

/* 头文件引用 */

/* 错误枚举定义 */
// clang-format off
typedef enum RESULT_Init
{
	ERR_Init_Finished 		= 0x00U,
	ERR_Init_Start				= 0x01U,
	ERR_Init_ERROR_GPIO		= 0x10U,
	ERR_Init_ERROR_TIM		= 0x11U,
	ERR_Init_ERROR_USART 	= 0x12U,
	ERR_Init_ERROR_PWM		= 0x13U,
	ERR_Init_ERROR_SPI		= 0x14U,
	ERR_Init_ERROR_ADC		= 0x15U,
	
} RESULT_Init;

typedef enum RESULT_RUN
{
	ERR_RUN_Finished 		= 0x00U,
	ERR_RUN_ERROR_UNST	= 0x01U,	/* 对应模块未初始化 */
	ERR_RUN_BUSY     		= 0x02U,
  ERR_RUN_TIMEOUT			= 0x03U,
	ERR_RUN_ERROR_UDIP	= 0x04U,	/* 未定义的输入参数 */
	ERR_RUN_ERROR_CALL	= 0x05U,	/* 错误的函数调用 */
	ERR_RUN_ERROR_ERIP  = 0x06U,	/* 错误的输入参数 */
} RESULT_RUN;

// clang-format on

#endif