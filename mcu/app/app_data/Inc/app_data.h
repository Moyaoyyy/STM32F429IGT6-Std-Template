/**
 * @file app_data.h
 * @brief 应用层共享数据模块头文件
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本模块提供各任务间共享的传感器数据结构和访问接口
 *       使用互斥量保护共享数据，确保多任务访问安全
 */

#ifndef __APP_DATA_H
#define __APP_DATA_H

#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>

/**
 * ============================================================================
 * 共享数据结构定义
 * ============================================================================
 */

/**
 * @brief 传感器数据结构体
 *
 * @note 包含温湿度和光照数据，供各任务共享访问
 *       数据更新由对应的采集任务完成
 *       数据读取由显示任务完成
 */
typedef struct
{
    /* 温湿度数据 (由Task_TempHum更新) */
    uint8_t temperature; /**< 温度值(单位: ℃) */
    uint8_t humidity;    /**< 湿度值(单位: %RH) */
    uint8_t dht11_valid; /**< DHT11数据有效标志(1=有效, 0=无效) */

    /* 光照数据 (由Task_Light更新) */
    uint32_t light_adc;  /**< 光敏电阻ADC原始值(0-4095) */
    uint8_t light_valid; /**< 光照数据有效标志(1=有效, 0=无效) */

} SensorData_TypeDef;

/**
 * ============================================================================
 * 外部变量声明
 * ============================================================================
 */

/* 全局传感器数据实例 */
extern SensorData_TypeDef g_SensorData;

/* 数据访问互斥量 */
extern SemaphoreHandle_t g_xDataMutex;

/**
 * ============================================================================
 * 函数声明
 * ============================================================================
 */

/**
 * @brief 初始化共享数据模块
 * @author Yukikaze
 *
 * @return BaseType_t 初始化结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 创建互斥量，初始化传感器数据结构
 *       需要在创建任务前调用
 */
BaseType_t AppData_Init(void);

/**
 * @brief 更新温湿度数据(线程安全)
 * @author Yukikaze
 *
 * @param temp 温度值
 * @param humi 湿度值
 * @param valid 数据有效标志
 */
void AppData_UpdateTempHum(uint8_t temp, uint8_t humi, uint8_t valid);

/**
 * @brief 更新光照数据(线程安全)
 * @author Yukikaze
 *
 * @param adc_value ADC原始值
 * @param valid 数据有效标志
 */
void AppData_UpdateLight(uint32_t adc_value, uint8_t valid);

/**
 * @brief 获取传感器数据副本(线程安全)
 * @author Yukikaze
 *
 * @param pData 指向数据结构的指针，用于存储读取的数据副本
 */
void AppData_GetSensorData(SensorData_TypeDef *pData);

#endif /* __APP_DATA_H */
