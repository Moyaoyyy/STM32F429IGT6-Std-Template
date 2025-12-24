#ifndef __BSP_OLED_DEBUG_H
#define __BSP_OLED_DEBUG_H

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* 软件/硬件IIC切换宏 0：软件 1：硬件 */
#define IIC_SELECT 1

#define OLED_ID          0x78
#define OLED_WR_CMD      0x00
#define OLED_WR_DATA     0x40

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize);

#endif
